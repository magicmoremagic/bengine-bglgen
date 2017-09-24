#include "lexer.hpp"
#include <be/core/logging.hpp>

namespace be::bglgen {
namespace {
%%{
   machine lexer;
   variable p p_;
   variable pe pe_;
   variable ts ts_;
   variable te te_;
   write data;
#/*

   odigit =                   [0-7] ;

   whitespace =               [\t ]+ ;
   optional_whitespace =      [\t ]* ;
   newline =                  '\\'? ( '\r' '\n'? | '\n' ) ;

   single_line_comment =      '//' [^\r\n]* ;
   multi_line_comment =       '/*' ( ^'*' | '*'+ [^/*] )* '*'+ '/' ;
   unclosed_comment =         '/*' ( ^'*' | '*'+ [^/*] )* '*'*;

   initial_identifier_char =  alpha | '_' ;
   identifier_char =          initial_identifier_char | digit ;
   bgl_identifier_char =      identifier_char | '.' | '-' ;

   gl_identifier =            'GL_' identifier_char* | 'gl' [A-Z] identifier_char* ;
   identifier =               initial_identifier_char identifier_char* ;
   non_gl_identifier =        identifier - gl_identifier ;
   bgl_identifier =           bgl_identifier_char+ ;

   bgl_prequel =              '//' optional_whitespace '#bgl' whitespace ;

   bgl_stop =                 bgl_prequel 'stop' ;
   bgl_checked =              bgl_prequel 'checked' optional_whitespace '(' optional_whitespace bgl_identifier optional_whitespace ')' ;
   bgl_unchecked =            bgl_prequel 'unchecked' ;
   bgl_weight =               bgl_prequel 'weight' optional_whitespace '(' optional_whitespace digit+ optional_whitespace ')' ;
   bgl_reset_weight =         bgl_prequel 'weight' ;
   bgl_malformed =            bgl_prequel identifier ;

   escape_seq =               '\\' ( [abfnrtv?\\'"] | odigit{1,3} | 'x' xdigit+ | 'u' xdigit{4} | 'U' xdigit{8} ) ;
   s_char =                   alnum | [ \t\v\f_{}\[\]#()<>%:;.?*+\-/^&|~!=,\'] | ^ascii | escape_seq ;
   string_literal =           ( 'u8' | [uULR] )? '"' s_char* '"' ;

   main := |*
      non_gl_identifier => {};
      gl_identifier => { symbol_(); };
      bgl_stop => { ignore_symbols_ = true; };
      bgl_checked => { set_check_(); };
      bgl_unchecked => { check_ = S(); };
      bgl_weight => { set_weight_(); };
      bgl_reset_weight => { weight_ = 100; };
      bgl_malformed => { bgl_malformed_(); };
      newline => { ls_ = te_; ++line_; };
      string_literal => {};
      single_line_comment => {};
      multi_line_comment => { comment_(); };
      unclosed_comment => {};
      any => {};
   *|;
#*/
}%%
} // be::bglgen::()

///////////////////////////////////////////////////////////////////////////////
Lexer::Lexer(Path path, SV input, std::unordered_multimap<S, SymbolUsage>& output)
   : path_(std::move(path)),
     input_(input),
	 output_(output),
	 ignore_symbols_(false) { }

///////////////////////////////////////////////////////////////////////////////
void Lexer::operator()() {
   check_ = S();
   weight_ = 100;
   ignore_symbols_ = false;
   ps_ = p_ = input_.data();
   pe_ = p_ + input_.length();
   const char* eof = pe_;
   int cs;
   int act;
   ls_ = p_;
   line_ = 1;
   %% write init;
   %% write exec;

   if (cs == lexer_error) {
      throw std::runtime_error("Lexer error!");
   }
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::symbol_() {
   if (ignore_symbols_) return;
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   SV symbol = input_.substr(offset, length);
   output_.insert(std::make_pair(S(symbol.begin(), symbol.end()), SymbolUsage { path_, line_, weight_, check_ }));
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::set_check_() {
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   SV symbol = input_.substr(offset, length);
   
   auto begin = symbol.end(), end = symbol.end();
   for (auto it = symbol.end() - 1; true; --it) {
      char c = *it;
      if (c >= 'a' && c <= 'z' ||
          c >= 'A' && c <= 'Z' ||
          c >= '0' && c <= '9' ||
          c == '_' || c == '.' || c == '-') {
         --begin;
      } else if (begin == end) {
         --begin;
         --end;
      } else {
         break;
      }
   }

   check_ = S(begin, end);
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::set_weight_() {
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   SV symbol = input_.substr(offset, length);
   
   U16 weight = 0;
   for (auto it = symbol.begin(), end = symbol.end(); it != end; ++it) {
      char c = *it;
      if (c < '0' || c > '9') {
	     if (weight > 0) break;
	  } else {
	     U16 new_weight = weight * 10 + c - '0';
		 if (new_weight > weight) {
		    weight = new_weight;
		 }
	  }
   }

   weight_ = weight;
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::comment_() {
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   SV comment = input_.substr(offset, length);

   bool prev_was_cr = false;
   int i = 0;
   int ls = -1;

   for (char c : comment) {
      ++i;

      if (c == '\n') {
         if (!prev_was_cr) {
            ++line_;
         }
         ls = i;
      }

      if (c == '\r') {
         prev_was_cr = true;
         ++line_;
         ls = i;
      } else {
         prev_was_cr = false;
      }
   }

   if (ls > 0) {
      ls_ = ts_ + ls;
   }
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::bgl_malformed_() {
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   SV symbol = input_.substr(offset, length);
   be_warn("") << "Malformed //#bgl declaration!"
	  & attr("Declaration") << S(symbol)
	  & attr("Path") << path_.generic_string()
	  & attr("Line") << line_
	  | default_log();
}

} // be::bglgen
