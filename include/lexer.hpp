#pragma once
#ifndef BE_BGLGEN_LEXER_HPP_
#define BE_BGLGEN_LEXER_HPP_

#include <be/core/filesystem.hpp>
#include <gsl/string_span>
#include <unordered_map>
#include <unordered_set>

namespace be::bglgen {

///////////////////////////////////////////////////////////////////////////////
struct SymbolUsage {
   Path path;
   U32 line;
   U16 weight;
   S check;
};

///////////////////////////////////////////////////////////////////////////////
class Lexer final : Immovable {
public:
   Lexer(Path path, SV input, std::unordered_multimap<S, SymbolUsage>& output);
   void operator()();
private:
   void symbol_();
   void set_check_();
   void set_weight_();
   void comment_();
   void bgl_malformed_();

   Path path_;
   SV input_;
   std::unordered_multimap<S, SymbolUsage>& output_;

   const char* ps_;
   const char* p_;
   const char* pe_;
   const char* ts_;
   const char* te_;
   const char* ls_;
   U32 line_;
   U16 weight_;
   bool ignore_symbols_;
   S check_;
};

} // be::bglgen

#endif
