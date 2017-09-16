
/* #line 1 "tools\\bglgen\\meta\\lexer.rl" */
#include "lexer.hpp"
#include <be/core/logging.hpp>

namespace be::bglgen {
namespace {

/* #line 7 "tools\\bglgen\\src\\lexer.cpp" */
static const int lexer_start = 58;
static const int lexer_first_final = 58;
static const int lexer_error = -1;

static const int lexer_en_main = 58;


/* #line 58 "tools\\bglgen\\meta\\lexer.rl" */

} // be::bglgen::()

///////////////////////////////////////////////////////////////////////////////
Lexer::Lexer(Path path, gsl::string_span<> input, std::unordered_multimap<S, SymbolUsage>& output)
   : path_(std::move(path)),
     input_(input),
	 output_(output) { }

///////////////////////////////////////////////////////////////////////////////
void Lexer::operator()() {
   check_ = S();
   weight_ = 100;
   ps_ = p_ = input_.data();
   pe_ = p_ + input_.length();
   char* eof = pe_;
   int cs;
   int act;
   ls_ = p_;
   line_ = 1;
   
/* #line 33 "tools\\bglgen\\src\\lexer.cpp" */
	{
	cs = lexer_start;
	( ts_) = 0;
	( te_) = 0;
	act = 0;
	}

/* #line 79 "tools\\bglgen\\meta\\lexer.rl" */
   
/* #line 39 "tools\\bglgen\\src\\lexer.cpp" */
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
	case 12:
	{{( p_) = ((( te_)))-1;}}
	break;
	}
	}
	goto st58;
tr2:
/* #line 51 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;}
	goto st58;
tr12:
/* #line 55 "tools\\bglgen\\meta\\lexer.rl" */
	{{( p_) = ((( te_)))-1;}}
	goto st58;
tr45:
/* #line 46 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ set_check_(); }}
	goto st58;
tr54:
/* #line 47 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ check_ = S(); }}
	goto st58;
tr60:
/* #line 49 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ weight_ = 100; }}
	goto st58;
tr64:
/* #line 48 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ set_weight_(); }}
	goto st58;
tr66:
/* #line 55 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;}
	goto st58;
tr67:
/* #line 50 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ ls_ = te_; ++line_; }}
	goto st58;
tr77:
/* #line 44 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st58;
tr78:
/* #line 50 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;{ ls_ = te_; ++line_; }}
	goto st58;
tr79:
/* #line 55 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st58;
tr82:
/* #line 54 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st58;
tr84:
/* #line 53 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_)+1;{ comment_(); }}
	goto st58;
tr85:
/* #line 52 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;}
	goto st58;
tr88:
/* #line 45 "tools\\bglgen\\meta\\lexer.rl" */
	{( te_) = ( p_);( p_)--;{ symbol_(); }}
	goto st58;
st58:
/* #line 1 "NONE" */
	{( ts_) = 0;}
	if ( ++( p_) == ( pe_) )
		goto _test_eof58;
case 58:
/* #line 1 "NONE" */
	{( ts_) = ( p_);}
/* #line 106 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 10: goto tr67;
		case 13: goto st60;
		case 34: goto tr69;
		case 35: goto tr70;
		case 47: goto st63;
		case 71: goto st67;
		case 76: goto tr73;
		case 82: goto tr73;
		case 85: goto tr73;
		case 92: goto st71;
		case 96: goto tr66;
		case 103: goto st72;
		case 117: goto tr76;
	}
	if ( (*( p_)) < 91 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 64 )
			goto tr66;
	} else if ( (*( p_)) > 94 ) {
		if ( 123 <= (*( p_)) )
			goto tr66;
	} else
		goto tr66;
	goto st59;
st59:
	if ( ++( p_) == ( pe_) )
		goto _test_eof59;
case 59:
	if ( (*( p_)) == 96 )
		goto tr77;
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr77;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr77;
		} else if ( (*( p_)) >= 91 )
			goto tr77;
	} else
		goto tr77;
	goto st59;
st60:
	if ( ++( p_) == ( pe_) )
		goto _test_eof60;
case 60:
	if ( (*( p_)) == 10 )
		goto tr67;
	goto tr78;
tr69:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 55 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 12;}
	goto st61;
st61:
	if ( ++( p_) == ( pe_) )
		goto _test_eof61;
case 61:
/* #line 163 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 10: goto tr79;
		case 34: goto tr2;
		case 36: goto tr79;
		case 64: goto tr79;
		case 92: goto st1;
		case 96: goto tr79;
		case 127: goto tr79;
	}
	if ( (*( p_)) > 8 ) {
		if ( 13 <= (*( p_)) && (*( p_)) <= 31 )
			goto tr79;
	} else if ( (*( p_)) >= 0 )
		goto tr79;
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
tr70:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
	goto st62;
st62:
	if ( ++( p_) == ( pe_) )
		goto _test_eof62;
case 62:
/* #line 333 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 9: goto st10;
		case 32: goto st10;
		case 112: goto st11;
	}
	goto tr79;
st10:
	if ( ++( p_) == ( pe_) )
		goto _test_eof10;
case 10:
	switch( (*( p_)) ) {
		case 9: goto st10;
		case 32: goto st10;
		case 112: goto st11;
	}
	goto tr12;
st11:
	if ( ++( p_) == ( pe_) )
		goto _test_eof11;
case 11:
	if ( (*( p_)) == 114 )
		goto st12;
	goto tr12;
st12:
	if ( ++( p_) == ( pe_) )
		goto _test_eof12;
case 12:
	if ( (*( p_)) == 97 )
		goto st13;
	goto tr12;
st13:
	if ( ++( p_) == ( pe_) )
		goto _test_eof13;
case 13:
	if ( (*( p_)) == 103 )
		goto st14;
	goto tr12;
st14:
	if ( ++( p_) == ( pe_) )
		goto _test_eof14;
case 14:
	if ( (*( p_)) == 109 )
		goto st15;
	goto tr12;
st15:
	if ( ++( p_) == ( pe_) )
		goto _test_eof15;
case 15:
	if ( (*( p_)) == 97 )
		goto st16;
	goto tr12;
st16:
	if ( ++( p_) == ( pe_) )
		goto _test_eof16;
case 16:
	switch( (*( p_)) ) {
		case 9: goto st17;
		case 32: goto st17;
	}
	goto tr12;
st17:
	if ( ++( p_) == ( pe_) )
		goto _test_eof17;
case 17:
	switch( (*( p_)) ) {
		case 9: goto st17;
		case 32: goto st17;
		case 99: goto st18;
	}
	goto tr12;
st18:
	if ( ++( p_) == ( pe_) )
		goto _test_eof18;
case 18:
	if ( (*( p_)) == 111 )
		goto st19;
	goto tr12;
st19:
	if ( ++( p_) == ( pe_) )
		goto _test_eof19;
case 19:
	if ( (*( p_)) == 109 )
		goto st20;
	goto tr12;
st20:
	if ( ++( p_) == ( pe_) )
		goto _test_eof20;
case 20:
	if ( (*( p_)) == 109 )
		goto st21;
	goto tr12;
st21:
	if ( ++( p_) == ( pe_) )
		goto _test_eof21;
case 21:
	if ( (*( p_)) == 101 )
		goto st22;
	goto tr12;
st22:
	if ( ++( p_) == ( pe_) )
		goto _test_eof22;
case 22:
	if ( (*( p_)) == 110 )
		goto st23;
	goto tr12;
st23:
	if ( ++( p_) == ( pe_) )
		goto _test_eof23;
case 23:
	if ( (*( p_)) == 116 )
		goto st24;
	goto tr12;
st24:
	if ( ++( p_) == ( pe_) )
		goto _test_eof24;
case 24:
	switch( (*( p_)) ) {
		case 9: goto st24;
		case 32: goto st24;
		case 40: goto st25;
	}
	goto tr12;
st25:
	if ( ++( p_) == ( pe_) )
		goto _test_eof25;
case 25:
	switch( (*( p_)) ) {
		case 9: goto st25;
		case 32: goto st25;
		case 98: goto st26;
	}
	goto tr12;
st26:
	if ( ++( p_) == ( pe_) )
		goto _test_eof26;
case 26:
	if ( (*( p_)) == 103 )
		goto st27;
	goto tr12;
st27:
	if ( ++( p_) == ( pe_) )
		goto _test_eof27;
case 27:
	if ( (*( p_)) == 108 )
		goto st28;
	goto tr12;
st28:
	if ( ++( p_) == ( pe_) )
		goto _test_eof28;
case 28:
	switch( (*( p_)) ) {
		case 9: goto st28;
		case 32: goto st28;
		case 44: goto st29;
	}
	goto tr12;
st29:
	if ( ++( p_) == ( pe_) )
		goto _test_eof29;
case 29:
	switch( (*( p_)) ) {
		case 9: goto st29;
		case 32: goto st29;
		case 99: goto st30;
		case 117: goto st40;
		case 119: goto st49;
	}
	goto tr12;
st30:
	if ( ++( p_) == ( pe_) )
		goto _test_eof30;
case 30:
	if ( (*( p_)) == 104 )
		goto st31;
	goto tr12;
st31:
	if ( ++( p_) == ( pe_) )
		goto _test_eof31;
case 31:
	if ( (*( p_)) == 101 )
		goto st32;
	goto tr12;
st32:
	if ( ++( p_) == ( pe_) )
		goto _test_eof32;
case 32:
	if ( (*( p_)) == 99 )
		goto st33;
	goto tr12;
st33:
	if ( ++( p_) == ( pe_) )
		goto _test_eof33;
case 33:
	if ( (*( p_)) == 107 )
		goto st34;
	goto tr12;
st34:
	if ( ++( p_) == ( pe_) )
		goto _test_eof34;
case 34:
	if ( (*( p_)) == 101 )
		goto st35;
	goto tr12;
st35:
	if ( ++( p_) == ( pe_) )
		goto _test_eof35;
case 35:
	if ( (*( p_)) == 100 )
		goto st36;
	goto tr12;
st36:
	if ( ++( p_) == ( pe_) )
		goto _test_eof36;
case 36:
	switch( (*( p_)) ) {
		case 9: goto st36;
		case 32: goto st36;
		case 44: goto st37;
	}
	goto tr12;
st37:
	if ( ++( p_) == ( pe_) )
		goto _test_eof37;
case 37:
	switch( (*( p_)) ) {
		case 9: goto st37;
		case 32: goto st37;
		case 96: goto tr12;
	}
	if ( (*( p_)) < 91 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 64 )
			goto tr12;
	} else if ( (*( p_)) > 94 ) {
		if ( 123 <= (*( p_)) )
			goto tr12;
	} else
		goto tr12;
	goto st38;
st38:
	if ( ++( p_) == ( pe_) )
		goto _test_eof38;
case 38:
	switch( (*( p_)) ) {
		case 9: goto st39;
		case 32: goto st39;
		case 41: goto tr45;
		case 96: goto tr12;
	}
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr12;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr12;
		} else if ( (*( p_)) >= 91 )
			goto tr12;
	} else
		goto tr12;
	goto st38;
st39:
	if ( ++( p_) == ( pe_) )
		goto _test_eof39;
case 39:
	switch( (*( p_)) ) {
		case 9: goto st39;
		case 32: goto st39;
		case 41: goto tr45;
	}
	goto tr12;
st40:
	if ( ++( p_) == ( pe_) )
		goto _test_eof40;
case 40:
	if ( (*( p_)) == 110 )
		goto st41;
	goto tr12;
st41:
	if ( ++( p_) == ( pe_) )
		goto _test_eof41;
case 41:
	if ( (*( p_)) == 99 )
		goto st42;
	goto tr12;
st42:
	if ( ++( p_) == ( pe_) )
		goto _test_eof42;
case 42:
	if ( (*( p_)) == 104 )
		goto st43;
	goto tr12;
st43:
	if ( ++( p_) == ( pe_) )
		goto _test_eof43;
case 43:
	if ( (*( p_)) == 101 )
		goto st44;
	goto tr12;
st44:
	if ( ++( p_) == ( pe_) )
		goto _test_eof44;
case 44:
	if ( (*( p_)) == 99 )
		goto st45;
	goto tr12;
st45:
	if ( ++( p_) == ( pe_) )
		goto _test_eof45;
case 45:
	if ( (*( p_)) == 107 )
		goto st46;
	goto tr12;
st46:
	if ( ++( p_) == ( pe_) )
		goto _test_eof46;
case 46:
	if ( (*( p_)) == 101 )
		goto st47;
	goto tr12;
st47:
	if ( ++( p_) == ( pe_) )
		goto _test_eof47;
case 47:
	if ( (*( p_)) == 100 )
		goto st48;
	goto tr12;
st48:
	if ( ++( p_) == ( pe_) )
		goto _test_eof48;
case 48:
	switch( (*( p_)) ) {
		case 9: goto st48;
		case 32: goto st48;
		case 41: goto tr54;
	}
	goto tr12;
st49:
	if ( ++( p_) == ( pe_) )
		goto _test_eof49;
case 49:
	if ( (*( p_)) == 101 )
		goto st50;
	goto tr12;
st50:
	if ( ++( p_) == ( pe_) )
		goto _test_eof50;
case 50:
	if ( (*( p_)) == 105 )
		goto st51;
	goto tr12;
st51:
	if ( ++( p_) == ( pe_) )
		goto _test_eof51;
case 51:
	if ( (*( p_)) == 103 )
		goto st52;
	goto tr12;
st52:
	if ( ++( p_) == ( pe_) )
		goto _test_eof52;
case 52:
	if ( (*( p_)) == 104 )
		goto st53;
	goto tr12;
st53:
	if ( ++( p_) == ( pe_) )
		goto _test_eof53;
case 53:
	if ( (*( p_)) == 116 )
		goto st54;
	goto tr12;
st54:
	if ( ++( p_) == ( pe_) )
		goto _test_eof54;
case 54:
	switch( (*( p_)) ) {
		case 9: goto st54;
		case 32: goto st54;
		case 41: goto tr60;
		case 44: goto st55;
	}
	goto tr12;
st55:
	if ( ++( p_) == ( pe_) )
		goto _test_eof55;
case 55:
	switch( (*( p_)) ) {
		case 9: goto st55;
		case 32: goto st55;
	}
	if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
		goto st56;
	goto tr12;
st56:
	if ( ++( p_) == ( pe_) )
		goto _test_eof56;
case 56:
	switch( (*( p_)) ) {
		case 9: goto st57;
		case 32: goto st57;
		case 41: goto tr64;
	}
	if ( 48 <= (*( p_)) && (*( p_)) <= 57 )
		goto st56;
	goto tr12;
st57:
	if ( ++( p_) == ( pe_) )
		goto _test_eof57;
case 57:
	switch( (*( p_)) ) {
		case 9: goto st57;
		case 32: goto st57;
		case 41: goto tr64;
	}
	goto tr12;
st63:
	if ( ++( p_) == ( pe_) )
		goto _test_eof63;
case 63:
	switch( (*( p_)) ) {
		case 42: goto st64;
		case 47: goto st66;
	}
	goto tr79;
st64:
	if ( ++( p_) == ( pe_) )
		goto _test_eof64;
case 64:
	if ( (*( p_)) == 42 )
		goto st65;
	goto st64;
st65:
	if ( ++( p_) == ( pe_) )
		goto _test_eof65;
case 65:
	switch( (*( p_)) ) {
		case 42: goto st65;
		case 47: goto tr84;
	}
	goto st64;
st66:
	if ( ++( p_) == ( pe_) )
		goto _test_eof66;
case 66:
	switch( (*( p_)) ) {
		case 10: goto tr85;
		case 13: goto tr85;
	}
	goto st66;
st67:
	if ( ++( p_) == ( pe_) )
		goto _test_eof67;
case 67:
	switch( (*( p_)) ) {
		case 76: goto st68;
		case 96: goto tr77;
	}
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr77;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr77;
		} else if ( (*( p_)) >= 91 )
			goto tr77;
	} else
		goto tr77;
	goto st59;
st68:
	if ( ++( p_) == ( pe_) )
		goto _test_eof68;
case 68:
	if ( (*( p_)) == 95 )
		goto st69;
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr77;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 96 ) {
			if ( 123 <= (*( p_)) )
				goto tr77;
		} else if ( (*( p_)) >= 91 )
			goto tr77;
	} else
		goto tr77;
	goto st59;
st69:
	if ( ++( p_) == ( pe_) )
		goto _test_eof69;
case 69:
	if ( (*( p_)) == 96 )
		goto tr88;
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr88;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr88;
		} else if ( (*( p_)) >= 91 )
			goto tr88;
	} else
		goto tr88;
	goto st69;
tr73:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 44 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 1;}
	goto st70;
st70:
	if ( ++( p_) == ( pe_) )
		goto _test_eof70;
case 70:
/* #line 846 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 34: goto st0;
		case 96: goto tr77;
	}
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr77;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr77;
		} else if ( (*( p_)) >= 91 )
			goto tr77;
	} else
		goto tr77;
	goto st59;
st71:
	if ( ++( p_) == ( pe_) )
		goto _test_eof71;
case 71:
	switch( (*( p_)) ) {
		case 10: goto tr67;
		case 13: goto st60;
	}
	goto tr79;
st72:
	if ( ++( p_) == ( pe_) )
		goto _test_eof72;
case 72:
	switch( (*( p_)) ) {
		case 96: goto tr77;
		case 108: goto st73;
	}
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr77;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr77;
		} else if ( (*( p_)) >= 91 )
			goto tr77;
	} else
		goto tr77;
	goto st59;
st73:
	if ( ++( p_) == ( pe_) )
		goto _test_eof73;
case 73:
	if ( (*( p_)) == 96 )
		goto tr77;
	if ( (*( p_)) < 65 ) {
		if ( (*( p_)) > 47 ) {
			if ( 58 <= (*( p_)) && (*( p_)) <= 64 )
				goto tr77;
		} else if ( (*( p_)) >= 0 )
			goto tr77;
	} else if ( (*( p_)) > 90 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr77;
		} else if ( (*( p_)) >= 91 )
			goto tr77;
	} else
		goto st69;
	goto st59;
tr76:
/* #line 1 "NONE" */
	{( te_) = ( p_)+1;}
/* #line 44 "tools\\bglgen\\meta\\lexer.rl" */
	{act = 1;}
	goto st74;
st74:
	if ( ++( p_) == ( pe_) )
		goto _test_eof74;
case 74:
/* #line 920 "tools\\bglgen\\src\\lexer.cpp" */
	switch( (*( p_)) ) {
		case 34: goto st0;
		case 56: goto tr73;
		case 96: goto tr77;
	}
	if ( (*( p_)) < 58 ) {
		if ( 0 <= (*( p_)) && (*( p_)) <= 47 )
			goto tr77;
	} else if ( (*( p_)) > 64 ) {
		if ( (*( p_)) > 94 ) {
			if ( 123 <= (*( p_)) )
				goto tr77;
		} else if ( (*( p_)) >= 91 )
			goto tr77;
	} else
		goto tr77;
	goto st59;
	}
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
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
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
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
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 

	_test_eof: {}
	if ( ( p_) == eof )
	{
	switch ( cs ) {
	case 59: goto tr77;
	case 60: goto tr78;
	case 61: goto tr79;
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
	case 62: goto tr79;
	case 10: goto tr12;
	case 11: goto tr12;
	case 12: goto tr12;
	case 13: goto tr12;
	case 14: goto tr12;
	case 15: goto tr12;
	case 16: goto tr12;
	case 17: goto tr12;
	case 18: goto tr12;
	case 19: goto tr12;
	case 20: goto tr12;
	case 21: goto tr12;
	case 22: goto tr12;
	case 23: goto tr12;
	case 24: goto tr12;
	case 25: goto tr12;
	case 26: goto tr12;
	case 27: goto tr12;
	case 28: goto tr12;
	case 29: goto tr12;
	case 30: goto tr12;
	case 31: goto tr12;
	case 32: goto tr12;
	case 33: goto tr12;
	case 34: goto tr12;
	case 35: goto tr12;
	case 36: goto tr12;
	case 37: goto tr12;
	case 38: goto tr12;
	case 39: goto tr12;
	case 40: goto tr12;
	case 41: goto tr12;
	case 42: goto tr12;
	case 43: goto tr12;
	case 44: goto tr12;
	case 45: goto tr12;
	case 46: goto tr12;
	case 47: goto tr12;
	case 48: goto tr12;
	case 49: goto tr12;
	case 50: goto tr12;
	case 51: goto tr12;
	case 52: goto tr12;
	case 53: goto tr12;
	case 54: goto tr12;
	case 55: goto tr12;
	case 56: goto tr12;
	case 57: goto tr12;
	case 63: goto tr79;
	case 64: goto tr82;
	case 65: goto tr82;
	case 66: goto tr85;
	case 67: goto tr77;
	case 68: goto tr77;
	case 69: goto tr88;
	case 70: goto tr77;
	case 71: goto tr79;
	case 72: goto tr77;
	case 73: goto tr77;
	case 74: goto tr77;
	}
	}

	}

/* #line 80 "tools\\bglgen\\meta\\lexer.rl" */

   if (cs == lexer_error) {
      throw std::runtime_error("Lexer error!");
   }
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::symbol_() {
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
          c == '_') {
         --begin;
      } else if (begin == end) {
         --begin;
         --end;
      } else {
         break;
      }
      --it;
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

} // be::bglgen
