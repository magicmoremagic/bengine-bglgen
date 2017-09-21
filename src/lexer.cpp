
/* #line 1 "tools\\bglgen\\meta\\lexer.rl" */
#include "lexer.hpp"
#include <be/core/logging.hpp>

namespace be::bglgen {
namespace {

/* #line 7 "tools\\bglgen\\src\\lexer.cpp" */
static const int lexer_start = 10;
static const int lexer_first_final = 10;
static const int lexer_error = -1;

static const int lexer_en_main = 10;


/* #line 63 "tools\\bglgen\\meta\\lexer.rl" */

} // be::bglgen::()

///////////////////////////////////////////////////////////////////////////////
Lexer::Lexer(Path path, gsl::string_span<> input, std::unordered_multimap<S, SymbolUsage>& output)
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
   char* eof = pe_;
   int cs;
   int act;
   ls_ = p_;
   line_ = 1;
   
/* #line 35 "tools\\bglgen\\src\\lexer.cpp" */
	{
	cs = lexer_start;
	( ts_) = 0;
	( te_) = 0;
	act = 0;
	}

/* #line 86 "tools\\bglgen\\meta\\lexer.rl" */
   
/* #line 41 "tools\\bglgen\\src\\lexer.cpp" */
	{
	if ( ( p_) == ( pe_) )
		goto _test_eof;
	switch ( cs )
	{
tr0:
/* #line 1 "NONE" */
	{	switch( act ) {
	case 1:
	{{( p_) = ((( te_)))-1;}}
	break;
	case 3:
	{{( p_) = ((( te_)))-1;} ignore_symbols_ = true; }
	break;
	case 4:
	{{( p_) = ((( te_)))-1;} set_check_(); }
	break;
	case 5:
	{{( p_) = ((( te_)))-1;} check_ = S(); }
	break;
	case 6:
	{{( p_) = ((( te_)))-1;} set_weight_(); }
	break;
	case 7:
	{{( p_) = ((( te_)))-1;} weight_ = 100; }
	break;
	case 11:
	{{( p_) = ((( te_)))-1;}}
	break;
	case 14:
	{{( p_) = ((( te_)))-1;}}
	break;
	}
	}
	goto st10;
tr2:
/* #line 56 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;}
	goto st10;
tr12:
/* #line 60 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;}
	goto st10;
tr13:
/* #line 55 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ ls_ = te_; ++line_; }}
	goto st10;
tr23:
/* #line 55 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;{ ls_ = te_; ++line_; }}
	goto st10;
tr24:
/* #line 60 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st10;
tr27:
/* #line 59 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st10;
tr29:
/* #line 58 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ comment_(); }}
	goto st10;
tr30:
/* #line 57 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st10;
tr37:
/* #line 54 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;{ bgl_malformed_(); }}
	goto st10;
tr73:
/* #line 47 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st10;
tr76:
/* #line 48 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;{ symbol_(); }}
	goto st10;
st10:
/* #line 1 "NONE" */
	{( ts_) = 0;}
	if ( ++( p_) == ( pe_) )
		goto _test_eof10;
case 10:
/* #line 1 "NONE" */
	{( ts_) = ( p_);}
/* #line 114 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 10: goto tr13;
		case 13: goto st11;
		case 34: goto tr15;
		case 47: goto st13;
		case 71: goto st54;
		case 76: goto tr19;
		case 82: goto tr19;
		case 85: goto tr19;
		case 92: goto st58;
		case 95: goto st53;
		case 103: goto st59;
		case 117: goto tr22;
	}
	if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else if ( (*( p_)) >= 65 )
		goto st53;
	goto tr12;
st11:
	if ( ++( p_) == ( pe_) )
		goto _test_eof11;
case 11:
	if ( (*( p_)) == 10 )
		goto tr13;
	goto tr23;
tr15:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 60 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 14;}
	goto st12;
st12:
	if ( ++( p_) == ( pe_) )
		goto _test_eof12;
case 12:
/* #line 149 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 10: goto tr24;
		case 34: goto tr2;
		case 36: goto tr24;
		case 64: goto tr24;
		case 92: goto st1;
		case 96: goto tr24;
		case 127: goto tr24;
	}
	if ( (*( p_)) > 8 ) {
		if ( 13 <= (*( p_)) && (*( p_)) <= 31 )
			goto tr24;
	} else if ( (*( p_)) >= 0 )
		goto tr24;
	goto st0;
st0:
	if ( ++( p_) == ( pe_) )
		goto _test_eof0;
case 0:
	switch( (*( p_)) ) {
		case 10: goto tr0;
		case 34: goto tr2;
		case 36: goto tr0;
		case 64: goto tr0;
		case 92: goto st1;
		case 96: goto tr0;
		case 127: goto tr0;
	}
	if ( (*( p_)) > 8 ) {
		if ( 13 <= (*( p_)) && (*( p_)) <= 31 )
			goto tr0;
	} else if ( (*( p_)) >= 0 )
		goto tr0;
	goto st0;
st1:
	if ( ++( p_) == ( pe_) )
		goto _test_eof1;
case 1:
	switch( (*( p_)) ) {
		case 34: goto st0;
		case 39: goto st0;
		case 63: goto st0;
		case 85: goto st2;
		case 92: goto st0;
		case 102: goto st0;
		case 110: goto st0;
		case 114: goto st0;
		case 117: goto st6;
		case 120: goto st9;
	}
	if ( (*( p_)) < 97 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 55 )
			goto st0;
	} else if ( (*( p_)) > 98 ) {
		if ( 116 <= (*( p_)) && (*( p_)) <= 118 )
			goto st0;
	} else
		goto st0;
	goto tr0;
st2:
	if ( ++( p_) == ( pe_) )
		goto _test_eof2;
case 2:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st3;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st3;
	} else
		goto st3;
	goto tr0;
st3:
	if ( ++( p_) == ( pe_) )
		goto _test_eof3;
case 3:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st4;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st4;
	} else
		goto st4;
	goto tr0;
st4:
	if ( ++( p_) == ( pe_) )
		goto _test_eof4;
case 4:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st5;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st5;
	} else
		goto st5;
	goto tr0;
st5:
	if ( ++( p_) == ( pe_) )
		goto _test_eof5;
case 5:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st6;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st6;
	} else
		goto st6;
	goto tr0;
st6:
	if ( ++( p_) == ( pe_) )
		goto _test_eof6;
case 6:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st7;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st7;
	} else
		goto st7;
	goto tr0;
st7:
	if ( ++( p_) == ( pe_) )
		goto _test_eof7;
case 7:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st8;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st8;
	} else
		goto st8;
	goto tr0;
st8:
	if ( ++( p_) == ( pe_) )
		goto _test_eof8;
case 8:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st9;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st9;
	} else
		goto st9;
	goto tr0;
st9:
	if ( ++( p_) == ( pe_) )
		goto _test_eof9;
case 9:
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st0;
	} else if ( (*( p_)) > 70 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 102 )
			goto st0;
	} else
		goto st0;
	goto tr0;
st13:
	if ( ++( p_) == ( pe_) )
		goto _test_eof13;
case 13:
	switch( (*( p_)) ) {
		case 42: goto st14;
		case 47: goto st16;
	}
	goto tr24;
st14:
	if ( ++( p_) == ( pe_) )
		goto _test_eof14;
case 14:
	if ( (*( p_)) == 42 )
		goto st15;
	goto st14;
st15:
	if ( ++( p_) == ( pe_) )
		goto _test_eof15;
case 15:
	switch( (*( p_)) ) {
		case 42: goto st15;
		case 47: goto tr29;
	}
	goto st14;
st16:
	if ( ++( p_) == ( pe_) )
		goto _test_eof16;
case 16:
	switch( (*( p_)) ) {
		case 9: goto st16;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st16;
		case 35: goto st18;
	}
	goto tr31;
tr31:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 57 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 11;}
	goto st17;
tr51:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 50 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 4;}
	goto st17;
tr54:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 49 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 3;}
	goto st17;
tr62:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 51 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 5;}
	goto st17;
tr72:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 52 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 6;}
	goto st17;
st17:
	if ( ++( p_) == ( pe_) )
		goto _test_eof17;
case 17:
/* #line 373 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 10: goto tr0;
		case 13: goto tr0;
	}
	goto tr31;
st18:
	if ( ++( p_) == ( pe_) )
		goto _test_eof18;
case 18:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 98: goto st19;
	}
	goto tr31;
st19:
	if ( ++( p_) == ( pe_) )
		goto _test_eof19;
case 19:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 103: goto st20;
	}
	goto tr31;
st20:
	if ( ++( p_) == ( pe_) )
		goto _test_eof20;
case 20:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 108: goto st21;
	}
	goto tr31;
st21:
	if ( ++( p_) == ( pe_) )
		goto _test_eof21;
case 21:
	switch( (*( p_)) ) {
		case 9: goto st22;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st22;
	}
	goto tr31;
st22:
	if ( ++( p_) == ( pe_) )
		goto _test_eof22;
case 22:
	switch( (*( p_)) ) {
		case 9: goto st22;
		case 10: goto tr37;
		case 13: goto tr37;
		case 32: goto st22;
		case 99: goto st23;
		case 115: goto st33;
		case 117: goto st36;
		case 119: goto st44;
	}
	goto tr31;
st23:
	if ( ++( p_) == ( pe_) )
		goto _test_eof23;
case 23:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 104: goto st24;
	}
	goto tr31;
st24:
	if ( ++( p_) == ( pe_) )
		goto _test_eof24;
case 24:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 101: goto st25;
	}
	goto tr31;
st25:
	if ( ++( p_) == ( pe_) )
		goto _test_eof25;
case 25:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 99: goto st26;
	}
	goto tr31;
st26:
	if ( ++( p_) == ( pe_) )
		goto _test_eof26;
case 26:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 107: goto st27;
	}
	goto tr31;
st27:
	if ( ++( p_) == ( pe_) )
		goto _test_eof27;
case 27:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 101: goto st28;
	}
	goto tr31;
st28:
	if ( ++( p_) == ( pe_) )
		goto _test_eof28;
case 28:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 100: goto st29;
	}
	goto tr31;
st29:
	if ( ++( p_) == ( pe_) )
		goto _test_eof29;
case 29:
	switch( (*( p_)) ) {
		case 9: goto st29;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st29;
		case 40: goto st30;
	}
	goto tr31;
st30:
	if ( ++( p_) == ( pe_) )
		goto _test_eof30;
case 30:
	switch( (*( p_)) ) {
		case 9: goto st30;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st30;
		case 95: goto st31;
	}
	if ( (*( p_)) < 48 ) {
		if ( 45 <= (*( p_)) && (*( p_)) <= 46 )
			goto st31;
	} else if ( (*( p_)) > 57 ) {
		if ( (*( p_)) > 90 ) {
			if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
				goto st31;
		} else if ( (*( p_)) >= 65 )
			goto st31;
	} else
		goto st31;
	goto tr31;
st31:
	if ( ++( p_) == ( pe_) )
		goto _test_eof31;
case 31:
	switch( (*( p_)) ) {
		case 9: goto st32;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st32;
		case 41: goto tr51;
		case 95: goto st31;
	}
	if ( (*( p_)) < 48 ) {
		if ( 45 <= (*( p_)) && (*( p_)) <= 46 )
			goto st31;
	} else if ( (*( p_)) > 57 ) {
		if ( (*( p_)) > 90 ) {
			if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
				goto st31;
		} else if ( (*( p_)) >= 65 )
			goto st31;
	} else
		goto st31;
	goto tr31;
st32:
	if ( ++( p_) == ( pe_) )
		goto _test_eof32;
case 32:
	switch( (*( p_)) ) {
		case 9: goto st32;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st32;
		case 41: goto tr51;
	}
	goto tr31;
st33:
	if ( ++( p_) == ( pe_) )
		goto _test_eof33;
case 33:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 116: goto st34;
	}
	goto tr31;
st34:
	if ( ++( p_) == ( pe_) )
		goto _test_eof34;
case 34:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 111: goto st35;
	}
	goto tr31;
st35:
	if ( ++( p_) == ( pe_) )
		goto _test_eof35;
case 35:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 112: goto tr54;
	}
	goto tr31;
st36:
	if ( ++( p_) == ( pe_) )
		goto _test_eof36;
case 36:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 110: goto st37;
	}
	goto tr31;
st37:
	if ( ++( p_) == ( pe_) )
		goto _test_eof37;
case 37:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 99: goto st38;
	}
	goto tr31;
st38:
	if ( ++( p_) == ( pe_) )
		goto _test_eof38;
case 38:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 104: goto st39;
	}
	goto tr31;
st39:
	if ( ++( p_) == ( pe_) )
		goto _test_eof39;
case 39:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 101: goto st40;
	}
	goto tr31;
st40:
	if ( ++( p_) == ( pe_) )
		goto _test_eof40;
case 40:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 99: goto st41;
	}
	goto tr31;
st41:
	if ( ++( p_) == ( pe_) )
		goto _test_eof41;
case 41:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 107: goto st42;
	}
	goto tr31;
st42:
	if ( ++( p_) == ( pe_) )
		goto _test_eof42;
case 42:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 101: goto st43;
	}
	goto tr31;
st43:
	if ( ++( p_) == ( pe_) )
		goto _test_eof43;
case 43:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 100: goto tr62;
	}
	goto tr31;
st44:
	if ( ++( p_) == ( pe_) )
		goto _test_eof44;
case 44:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 101: goto st45;
	}
	goto tr31;
st45:
	if ( ++( p_) == ( pe_) )
		goto _test_eof45;
case 45:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 105: goto st46;
	}
	goto tr31;
st46:
	if ( ++( p_) == ( pe_) )
		goto _test_eof46;
case 46:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 103: goto st47;
	}
	goto tr31;
st47:
	if ( ++( p_) == ( pe_) )
		goto _test_eof47;
case 47:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 104: goto st48;
	}
	goto tr31;
st48:
	if ( ++( p_) == ( pe_) )
		goto _test_eof48;
case 48:
	switch( (*( p_)) ) {
		case 10: goto tr30;
		case 13: goto tr30;
		case 116: goto tr67;
	}
	goto tr31;
tr68:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 57 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 11;}
	goto st49;
tr67:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 53 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 7;}
	goto st49;
st49:
	if ( ++( p_) == ( pe_) )
		goto _test_eof49;
case 49:
/* #line 737 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 9: goto tr68;
		case 10: goto tr0;
		case 13: goto tr0;
		case 32: goto tr68;
		case 40: goto st50;
	}
	goto tr31;
st50:
	if ( ++( p_) == ( pe_) )
		goto _test_eof50;
case 50:
	switch( (*( p_)) ) {
		case 9: goto st50;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st50;
	}
	if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
		goto st51;
	goto tr31;
st51:
	if ( ++( p_) == ( pe_) )
		goto _test_eof51;
case 51:
	switch( (*( p_)) ) {
		case 9: goto st52;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st52;
		case 41: goto tr72;
	}
	if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
		goto st51;
	goto tr31;
st52:
	if ( ++( p_) == ( pe_) )
		goto _test_eof52;
case 52:
	switch( (*( p_)) ) {
		case 9: goto st52;
		case 10: goto tr30;
		case 13: goto tr30;
		case 32: goto st52;
		case 41: goto tr72;
	}
	goto tr31;
st53:
	if ( ++( p_) == ( pe_) )
		goto _test_eof53;
case 53:
	if ( (*( p_)) == 95 )
		goto st53;
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st53;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else
		goto st53;
	goto tr73;
st54:
	if ( ++( p_) == ( pe_) )
		goto _test_eof54;
case 54:
	switch( (*( p_)) ) {
		case 76: goto st55;
		case 95: goto st53;
	}
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st53;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else
		goto st53;
	goto tr73;
st55:
	if ( ++( p_) == ( pe_) )
		goto _test_eof55;
case 55:
	if ( (*( p_)) == 95 )
		goto st56;
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st53;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else
		goto st53;
	goto tr73;
st56:
	if ( ++( p_) == ( pe_) )
		goto _test_eof56;
case 56:
	if ( (*( p_)) == 95 )
		goto st56;
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st56;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st56;
	} else
		goto st56;
	goto tr76;
tr19:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 47 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 1;}
	goto st57;
st57:
	if ( ++( p_) == ( pe_) )
		goto _test_eof57;
case 57:
/* #line 854 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 34: goto st0;
		case 95: goto st53;
	}
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st53;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else
		goto st53;
	goto tr73;
st58:
	if ( ++( p_) == ( pe_) )
		goto _test_eof58;
case 58:
	switch( (*( p_)) ) {
		case 10: goto tr13;
		case 13: goto st11;
	}
	goto tr24;
st59:
	if ( ++( p_) == ( pe_) )
		goto _test_eof59;
case 59:
	switch( (*( p_)) ) {
		case 95: goto st53;
		case 108: goto st60;
	}
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st53;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else
		goto st53;
	goto tr73;
st60:
	if ( ++( p_) == ( pe_) )
		goto _test_eof60;
case 60:
	if ( (*( p_)) == 95 )
		goto st53;
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st53;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else
		goto st56;
	goto tr73;
tr22:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 47 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 1;}
	goto st61;
st61:
	if ( ++( p_) == ( pe_) )
		goto _test_eof61;
case 61:
/* #line 916 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 34: goto st0;
		case 56: goto tr19;
		case 95: goto st53;
	}
	if ( (*( p_)) < 65 ) {
		if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
			goto st53;
	} else if ( (*( p_)) > 90 ) {
		if ( 97 <= (*( p_)) && (*( p_)) <= 122 )
			goto st53;
	} else
		goto st53;
	goto tr73;
	}
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 

	_test_eof: {}
	if ( ( p_) == eof )
	{
	switch ( cs ) {
	case 11: goto tr23;
	case 12: goto tr24;
	case 0: goto tr0;
	case 1: goto tr0;
	case 2: goto tr0;
	case 3: goto tr0;
	case 4: goto tr0;
	case 5: goto tr0;
	case 6: goto tr0;
	case 7: goto tr0;
	case 8: goto tr0;
	case 9: goto tr0;
	case 13: goto tr24;
	case 14: goto tr27;
	case 15: goto tr27;
	case 16: goto tr30;
	case 17: goto tr0;
	case 18: goto tr30;
	case 19: goto tr30;
	case 20: goto tr30;
	case 21: goto tr30;
	case 22: goto tr37;
	case 23: goto tr30;
	case 24: goto tr30;
	case 25: goto tr30;
	case 26: goto tr30;
	case 27: goto tr30;
	case 28: goto tr30;
	case 29: goto tr30;
	case 30: goto tr30;
	case 31: goto tr30;
	case 32: goto tr30;
	case 33: goto tr30;
	case 34: goto tr30;
	case 35: goto tr30;
	case 36: goto tr30;
	case 37: goto tr30;
	case 38: goto tr30;
	case 39: goto tr30;
	case 40: goto tr30;
	case 41: goto tr30;
	case 42: goto tr30;
	case 43: goto tr30;
	case 44: goto tr30;
	case 45: goto tr30;
	case 46: goto tr30;
	case 47: goto tr30;
	case 48: goto tr30;
	case 49: goto tr0;
	case 50: goto tr30;
	case 51: goto tr30;
	case 52: goto tr30;
	case 53: goto tr73;
	case 54: goto tr73;
	case 55: goto tr73;
	case 56: goto tr76;
	case 57: goto tr73;
	case 58: goto tr24;
	case 59: goto tr73;
	case 60: goto tr73;
	case 61: goto tr73;
	}
	}

	}

/* #line 87 "tools\\bglgen\\meta\\lexer.rl" */

   if (cs == lexer_error) {
      throw std::runtime_error("Lexer error!");
   }
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::symbol_() {
   if (ignore_symbols_) return;
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   gsl::string_span<> symbol = input_.subspan(offset, length);
   output_.insert(std::make_pair(S(symbol.begin(), symbol.end()), SymbolUsage { path_, line_, weight_, check_ }));
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::set_check_() {
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   gsl::string_span<> symbol = input_.subspan(offset, length);
   
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
   gsl::string_span<> symbol = input_.subspan(offset, length);
   
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
   gsl::string_span<> comment = input_.subspan(offset, length);

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
   gsl::string_span<> symbol = input_.subspan(offset, length);
   be_warn("") << "Malformed //#bgl declaration!"
	  & attr("Declaration") << S(symbol.begin(), symbol.end())
	  & attr("Path") << path_.generic_string()
	  & attr("Line") << line_
	  | default_log();
}

} // be::bglgen
