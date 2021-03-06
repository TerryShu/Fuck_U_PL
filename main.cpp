# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <vector>
# include <ctype.h>
# include <iomanip>


using namespace std;

// ======================Data Struct==============================
enum Error_type{ UNRECOGNIZED, UNDEFINED, UNEXPECTED, ERROR };

struct Identify_Data{
  string identify_Name ;
  double identify_value ;
  bool isFloat ;
} ;

// ===============================================================

// ======================global var===============================
string gNowToken ;
string gPeekToken ;
string gBoolAns ;
vector<Identify_Data> gId ;
int gTestNum = 0 ;
bool gChange = false ;
bool gIsBoolOP = false ;
bool gQuit = false ;
bool gIsFloat = false ;
char gUnknowChar ;
// ===============================================================

// ======================use for Initialize=======================
void InitializeToken() ;
void InitializeState() ;
// ===============================================================

// =======================use for get token=======================
string GetToken() ;
void SkipExitChar() ;
string SpecialToken( char aChar, string Token ) ;
string ReadNum() ;
string ReadIdent() ;
string ReadOPAndComment() ;
bool IsFloat() ;
void TakeToken() ;
void ReadAfterError() ;
// ===============================================================

// =======================use for judge===========================
bool JudgeIDENT( string Token ) ;
bool JudgeNum( string Token ) ;
bool JudgeFloat( string Token ) ;
bool JudgeInt( string Token ) ;
int JudgeDefine( string id_name, bool take ) ;
bool IsQuit( string Token ) ;
// ===============================================================

// =======================recursive===============================
void Command() ;
double IDlessArithExpOrBexp( string id_name ) ;
string BooleanOprator() ;
double NOT_ID_StartArithExpOrBexp() ;
double NOT_ID_StartArithExp() ;
double NOT_ID_StartTerm() ;
double NOT_ID_StartFactor() ;
double ArithExp() ;
double Term() ;
double Factor() ;
// ===============================================================
void Output_ans( double ans ) ;
void Update_define( string id_name, double value ) ;

int main() {
  cin >> gTestNum ;
  cout << "Program starts..." << endl ;

  while ( !gQuit ) {
    InitializeState() ;
    try {
      Command() ;
    } // try
    catch ( Error_type error ) {
      ReadAfterError() ;
      if ( error == UNRECOGNIZED ) {
        cout << "Unrecognized token with first char : '" << gUnknowChar << "'" << endl ;
      } // if
      else if ( error == UNDEFINED ) {
        cout << "Undefined identifier : '" << gNowToken << "'" << endl ;
      } // else if
      else if ( error == UNEXPECTED ) {
        cout << "Unexpected token : '" << gNowToken << "'" << endl ;
      } // else if
      else if ( error == ERROR ) {
        cout << "Error" << endl ;
        return 0 ;
      } // else if
    } // catch
  } // while

  cout << "Program exits..." << endl ;
} // main()

string GetToken() {
  InitializeToken() ;
  char aChar ;
  char peekChar ;
  SkipExitChar() ; // 跳過一開始空白
  string token = "" ;
  peekChar = cin.peek() ;
  if ( isalnum( peekChar ) || peekChar == '_' || peekChar == '.' ) {

    if ( isdigit( peekChar ) || peekChar == '.' ) {
      token = ReadNum() ;
    } // if
    else {
      token = ReadIdent() ;
    } // else

  } // if
  else if ( peekChar == '+' || peekChar == '=' || peekChar == ':' ||
            peekChar == '-' || peekChar == '*' || peekChar == '/' ||
            peekChar == ';' || peekChar == '>' || peekChar == '<' ||
            peekChar == '(' || peekChar == ')' ) {
    token = ReadOPAndComment() ;
  } // else if
  else {
    if ( !isalnum( peekChar ) && peekChar != '_' && peekChar != '+' &&
         peekChar != '-' && peekChar != '*' && peekChar != '/' &&
         peekChar != ';' && peekChar != '>' && peekChar != '<' &&
         peekChar != ':' && peekChar != '(' && peekChar != ')' &&
         peekChar != '.' && peekChar != '=' ) {
      gUnknowChar = peekChar ;
      throw UNRECOGNIZED ;
    } // if all possibly start

  } // else



  return token ;

} // GetToken()

string ReadNum() {
  char aChar ;
  char peekChar ;
  string token ;
  token.clear() ;

  peekChar = cin.peek() ;

  if ( peekChar == '.' ) {
    aChar = cin.get() ; // read '.'
    token = token + aChar ;
    peekChar = cin.peek() ;
    while ( isdigit( peekChar ) ) {
      aChar = cin.get() ;
      token = token + aChar ;
      peekChar = cin.peek() ;
    } // while
  } // if
  else {
    while ( isdigit( peekChar ) ) {
      aChar = cin.get() ;
      token = token + aChar ;
      peekChar = cin.peek() ;
    } // while

    if ( peekChar == '.' ) {
      aChar = cin.get() ; // read '.'
      token = token + aChar ;
      peekChar = cin.peek() ;
      while ( isdigit( peekChar ) ) {
        aChar = cin.get() ;
        token = token + aChar ;
        peekChar = cin.peek() ;
      } // while
    } // if

  } // else


  return token ;
} // ReadNum()

string ReadIdent() {
  char aChar ;
  char peekChar ;
  string token ;
  token.clear() ;

  peekChar = cin.peek() ;
  while ( isalnum( peekChar ) || peekChar == '_' ) {
    aChar = cin.get() ;
    token = token + aChar ;
    peekChar = cin.peek() ;
  } // while

  return token ;
} // ReadIdent()

string ReadOPAndComment() {
  char peekChar ;
  char aChar ;
  string token ;
  token.clear() ;
  peekChar = cin.peek() ;

  if ( peekChar == '+' || peekChar == '-' || peekChar == '*' || peekChar == '(' ||
       peekChar == ')' || peekChar == ';' || peekChar == '=' ) {
    aChar = cin.get() ;
    token = token + aChar ;
    return token ;
  } // if
  else if ( peekChar == '/' ) {
    aChar = cin.get() ;
    token = token + aChar ;
    peekChar = cin.peek() ;
    if ( peekChar == '/' ) {
      char commit[500] ; // 註解
      cin.getline( commit, 500 ) ;
      token = GetToken() ;
    } // if comment

    return token ;
  } // else if
  else if ( peekChar == ':' ) {
    aChar = cin.get() ;
    token = token + aChar ;
    peekChar = cin.peek() ;
    if ( peekChar == '=' ) {
      aChar = cin.get() ;
      token = token + aChar ;
    } // if

    return token ;
  } // else if
  else if ( peekChar == '>' || peekChar == '<' ) {
    aChar = cin.get() ;
    token = token + aChar ;
    if ( aChar == '>' ) {
      peekChar = cin.peek() ;
      if ( peekChar == '=' ) {
        aChar = cin.get() ;
        token = token + aChar ;
      } // if
    } // if
    else if ( aChar == '<' ) {
      peekChar = cin.peek() ;
      if ( peekChar == '=' || peekChar == '>' ) {
        aChar = cin.get() ;
        token = token + aChar ;
      } // if
    } // else if

    return token ;
  } // else if
  else {
    cout << "READ " ;
    throw  ERROR ;
  } // else


} // ReadOPAndComment()

void SkipExitChar() {
  char aChar ;
  char exitChar ;
  aChar = cin.peek() ;

  if ( aChar != ' ' && aChar != '\n' && aChar != '\0' && aChar != '\t' ) {
    return ;
  } // if
  else {

    while ( aChar == ' ' || aChar == '\n' || aChar == '\0' || aChar == '\t' ) {
      exitChar = cin.get() ;
      aChar = cin.peek() ;
    } // while

  } // else

} // SkipExitChar()

void InitializeToken() {
  gNowToken.clear() ;
} // InitializeToken()

void InitializeState() {
  gNowToken.clear() ;
  gPeekToken.clear() ;
  gBoolAns.clear() ;
  gIsFloat = false ;
  gQuit = false ;
  gIsBoolOP = false ;
  gChange = false ;
  gUnknowChar = '\0' ;
} // InitializeState()

bool IsQuit( string Token ) {
  if ( Token == "quit" ) {
    gQuit = true ;
    return true ;
  } // if

  return false ;
} // IsQuit()

void Output_ans( double ans ) {

  if ( !gIsBoolOP ) {
    if ( gIsFloat ) {
      if ( ans > -0.001 && ans < 0 )
        cout << fixed << setprecision( 3 ) << "0.000" << endl ;
      else
        cout << fixed << setprecision( 3 ) << ans << endl ;
    } // if
    else {
      cout << ( int ) ans << endl ;
    } // else
  } // if
  else {
    cout << gBoolAns << endl ;
  } // else

} // Output_ans()

void Update_define( string id_name, double value ) {
  for ( int i = 0 ; i < gId.size() ; i++ ) {
    if ( gId[i].identify_Name == id_name ) {
      gId[i].identify_value = value ;

      if ( gIsFloat ) {
        gId[i].isFloat = true ;
      } // if
      else {
        gId[i].isFloat = false ;
      } // else

      return ;
    } // if vector has this var
  } // for

  Identify_Data tmp ;
  tmp.identify_Name = id_name ;
  tmp.identify_value = value ;
  if ( gIsFloat ) {
    tmp.isFloat = true ;
  } // if
  else {
    tmp.isFloat = false ;
  } // else

  gId.push_back( tmp ) ;

} // Update_define()

void Command() {
  cout << "> " ;
  gNowToken = GetToken() ;
  double return_ans ;
  if ( IsQuit( gNowToken ) ) {
    gQuit = true ;
  } // if quit
  else if ( JudgeIDENT( gNowToken ) ) {
    string id_name = gNowToken ;
    TakeToken() ; // take IDENT
    if ( gNowToken.empty() ) {
      gNowToken = GetToken() ;
    } // if

    if ( gNowToken == ":=" ) {
      TakeToken() ; // take ":="
      double return_ans = ArithExp() ;

      if ( gNowToken.empty() ) {
        gNowToken = GetToken() ; // get ";"
      } // if

      if ( gNowToken == ";" ) {
        TakeToken() ; // take ";"
        Update_define( id_name, return_ans ) ;
        Output_ans( return_ans ) ;
      } // if
      else {
        throw UNEXPECTED ;
      } // else

    } // if ':=' <ArithExp>
    else {
      double return_ans = IDlessArithExpOrBexp( id_name ) ;

      if ( gNowToken.empty() ) {
        gNowToken = GetToken() ; // get ";"
      } // if

      if ( gNowToken == ";" ) {
        TakeToken() ; // take ";"
        if ( !gChange ) {
          int ident_index = JudgeDefine( id_name, false ) ;
          if ( gId[ident_index].isFloat ) gIsFloat = true ;

          return_ans = gId[ident_index].identify_value ;
        } // if

        Output_ans( return_ans ) ;
      } // if
      else {
        throw UNEXPECTED ;
      } // else

    } // else <IDlessArithExpOrBexp>

  } // if IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'
  else if ( !JudgeIDENT( gNowToken ) ) {
    return_ans = NOT_ID_StartArithExpOrBexp() ;

    if ( gNowToken.empty() ) {
      gNowToken = GetToken() ; // get ";"
    } // if

    if ( gNowToken == ";" ) {
      TakeToken() ; // take ";"
      Output_ans( return_ans ) ;
    } // if
    else {
      throw UNEXPECTED ;
    } // else

  } // else if <NOT_IDStartArithExpOrBexp>
  else {
    throw UNEXPECTED ;
  } // else

} // Command()

bool JudgeIDENT( string Token ) {
  int length = Token.length() ;
  int correct = 0 ;

  if ( isalpha( Token.at( 0 ) ) || Token.at( 0 ) == '_' ) {
    correct++ ;
  } // if a~z||A~Z|| _

  for ( int i = 1 ; i < length ; i++ ) {
    if ( isalnum( Token.at( i ) ) || Token.at( i ) == '_' ) {
      correct++ ;
    } // if  a~z || A~Z || 0~9 || _
  } // for

  if ( correct == length ) return true ;

  return false ;

} // JudgeIDENT()

bool JudgeNum( string Token ) {
  if ( JudgeFloat( Token ) || JudgeInt( Token ) ) {
    return true ;
  } // if

  return false ;
} // JudgeNum()

bool JudgeInt( string Token ) {
  int length = Token.length() ;
  int correct = 0 ;
  for ( int i = 0 ; i < length ; i++ ) {
    if ( Token.at( i ) >= '0' && Token.at( i ) <= '9' ) {
      correct++ ;
    } // if  0~9
  } // for

  if ( correct == length ) return true ;

  return false ;
} // JudgeInt()

bool JudgeFloat( string Token ) {
  int length = Token.length() ;
  int correct = 0 ;
  bool find_point = false ;

  if ( Token == "." ) {
    return false ;
  } // if just one point '.'

  for ( int i = 0 ; i < length ; i++ ) {
    if ( Token.at( i ) >= '0' && Token.at( i ) <= '9' ) {
      correct++ ;
    } // if  0~9
    else if ( Token.at( i ) == '.' && !find_point ) {
      correct++ ;
      find_point = true ;
    } // else if judge 123.111 && .2 && 2. should take care
  } // for

  if ( correct == length && find_point ) return true ;

  return false ;
} // JudgeFloat()

int JudgeDefine( string id_name, bool take ) {

  for ( int i = 0 ; i < gId.size() ; i++ ) {
    if ( gId[i].identify_Name == id_name ) {
      if ( gId[i].isFloat ) {
        gIsFloat = true ;
      } // if

      if ( take ) {
        TakeToken() ; // take IDENT
      } // if

      return i ;
    } // if vector has this var
  } // for

  if ( !take ) {
    gNowToken = id_name ;
  } // if

  throw UNDEFINED ;
  return 0 ;

} // JudgeDefine()

void ReadAfterError() {
  char aChar ;
  aChar = cin.peek() ;
  while ( aChar != '\n' ) {
    aChar = cin.get() ;
    aChar = cin.peek() ;
  } // while
} // ReadAfterError()

void TakeToken() {
  if ( gPeekToken.empty() ) {
    gNowToken.clear() ;
  } // if
  else {
    gNowToken = gPeekToken ;
    gPeekToken.clear() ;
  } // else
} // TakeToken()

double NOT_ID_StartArithExpOrBexp() {
  double return_ans = NOT_ID_StartArithExp() ;

  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  string boolOperator = BooleanOprator() ;
  if ( boolOperator != "Not_boolOP" ) {
    double arithexp_return = ArithExp() ;
    gIsBoolOP = true ;

    if ( ( ( return_ans - arithexp_return ) <= 0.00011 && ( return_ans > arithexp_return ) ) ||
         ( ( arithexp_return - return_ans ) <= 0.00011 && ( arithexp_return > return_ans ) )  ) {

      return_ans = arithexp_return ;

    } // if

    if ( boolOperator == "=" ) {
      if ( return_ans == arithexp_return ) gBoolAns = "true"  ;
      else gBoolAns = "false" ;
    } // if "="
    else if ( boolOperator == "<>" ) {
      if ( return_ans != arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if "<>"
    else if ( boolOperator == ">" ) {
      if ( return_ans > arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if ">"
    else if ( boolOperator == "<" ) {
      if ( return_ans < arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if "<"
    else if ( boolOperator == ">=" ) {
      if ( return_ans >= arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if ">="
    else if ( boolOperator == "<=" ) {
      if ( return_ans <= arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if "<="

  } // if [ <BooleanOperator> <ArithExp> ]

  return return_ans ;

} // NOT_ID_StartArithExpOrBexp()

double IDlessArithExpOrBexp( string id_name ) {
  double return_ans ;

  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  if ( gNowToken == "+" || gNowToken == "-" ||
       gNowToken == "*" || gNowToken == "/" ) {

    gChange = true ;
    int ident_index = JudgeDefine( id_name, false ) ;
    if ( gId[ident_index].isFloat ) gIsFloat = true ;

    return_ans = gId[ident_index].identify_value ;

    while ( gNowToken == "+" || gNowToken == "-" ||
            gNowToken == "*" || gNowToken == "/" ) {

      string four = gNowToken ;
      TakeToken() ; // take * or /

      if ( four == "*" ) {
        double factor_return = Factor() ;
        return_ans *= factor_return ;

        if ( !gIsFloat ) {
          return_ans = ( int ) return_ans ;
        } // if

      } // if '*' <Factor>
      else if ( four == "/" ) {
        double factor_return = Factor() ;
        if ( factor_return == 0 ) {
          throw ERROR ;
        } // if Don't /0

        return_ans /= factor_return ;

        if ( !gIsFloat ) {
          return_ans = ( int ) return_ans ;
        } // if

      } // else if '/' <Factor>
      else if ( four == "+" ) {
        double term_return = Term() ;
        return_ans += term_return ;
      } // if '+' <Term>
      else if ( four == "-" ) {
        double term_return = Term() ;
        return_ans -= term_return ;
      } // else if '-' <Term>

      if ( gNowToken.empty() ) {
        gNowToken = GetToken();
      } // if

    } // while

  } // if

  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  string boolOperator = BooleanOprator() ;
  if ( boolOperator != "Not_boolOP" ) {
    if ( !gChange ) {
      int ident_index = JudgeDefine( id_name, false ) ;
      if ( gId[ident_index].isFloat ) gIsFloat = true ;

      return_ans = gId[ident_index].identify_value ;
    } // if

    gChange = true ;
    double arithexp_return = ArithExp() ;
    gIsBoolOP = true ;

    if ( ( ( return_ans - arithexp_return ) <= 0.00011 && ( return_ans > arithexp_return ) ) ||
         ( ( arithexp_return - return_ans ) <= 0.00011 && ( arithexp_return > return_ans ) )  ) {

      return_ans = arithexp_return ;

    } // if

    if ( boolOperator == "=" ) {
      if ( return_ans == arithexp_return ) gBoolAns = "true"  ;
      else gBoolAns = "false" ;
    } // if "="
    else if ( boolOperator == "<>" ) {
      if ( return_ans != arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if "<>"
    else if ( boolOperator == ">" ) {
      if ( return_ans > arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if ">"
    else if ( boolOperator == "<" ) {
      if ( return_ans < arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if "<"
    else if ( boolOperator == ">=" ) {
      if ( return_ans >= arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if ">="
    else if ( boolOperator == "<=" ) {
      if ( return_ans <= arithexp_return ) gBoolAns = "true" ;
      else gBoolAns = "false" ;
    } // else if "<="

  } // if [ <BooleanOperator> <ArithExp> ]

  return return_ans ;
} // IDlessArithExpOrBexp()

string BooleanOprator() {
  if ( gNowToken == "=" ) {
    TakeToken() ;
    return "=" ;
  } // if
  else if ( gNowToken == "<>" ) {
    TakeToken() ;
    return "<>" ;
  } // else if
  else if ( gNowToken == ">" ) {
    TakeToken() ;
    return ">" ;
  } // else if
  else if ( gNowToken == "<" ) {
    TakeToken() ;
    return "<" ;
  } // else if
  else if ( gNowToken == ">=" ) {
    TakeToken() ;
    return ">=" ;
  } // else if
  else if ( gNowToken == "<=" ) {
    TakeToken() ;
    return "<=" ;
  } // else if
  else {
    return "Not_boolOP" ;
  } // else

} // BooleanOprator()

double NOT_ID_StartArithExp() {
  double return_ans = NOT_ID_StartTerm() ;
  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  if ( gNowToken == "+" || gNowToken == "-" ) {

    while ( gNowToken == "+" || gNowToken == "-" ) {
      string addOrSub = gNowToken ;
      TakeToken() ; // take + or -
      double term_return = Term() ;

      if ( addOrSub == "+" ) {
        return_ans += term_return ;
      } // if '+' <Term>
      else if ( addOrSub == "-" ) {
        return_ans -= term_return ;
      } // else if '-' <Term>

      if ( gNowToken.empty() ) {
        gNowToken = GetToken() ;
      } // if

    } // while

  } // if

  return return_ans ;
} // NOT_ID_StartArithExp()

double NOT_ID_StartTerm() {
  double return_ans = NOT_ID_StartFactor() ;

  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  if ( gNowToken == "*" || gNowToken == "/" ) {

    while ( gNowToken == "*" || gNowToken == "/" ) {
      string multiOrDiv = gNowToken ;
      TakeToken() ; // take * or /
      double factor_return = Factor() ;
      if ( multiOrDiv == "*" ) {
        return_ans *= factor_return ;

        if ( !gIsFloat ) {
          return_ans = ( int ) return_ans ;
        } // if

      } // if '*' <Factor>
      else if ( multiOrDiv == "/" ) {
        if ( factor_return == 0 ) {
          throw ERROR ;
        } // if Don't /0

        return_ans /= factor_return ;

        if ( !gIsFloat ) {
          return_ans = ( int ) return_ans ;
        } // if

      } // else if '/' <Factor>

      if ( gNowToken.empty() ) {
        gNowToken = GetToken() ;
      } // if

    } // while

  } // if

  return return_ans ;
} // NOT_ID_StartTerm()

double NOT_ID_StartFactor() {
  double return_ans = 0.0 ;

  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if


  if ( gNowToken == "+" || gNowToken == "-" ) {
    string sign = gNowToken ;
    TakeToken() ; // take [sign]
    if ( gNowToken.empty() ) {
      gNowToken = GetToken() ; // get Num
    } // if

    if ( JudgeNum( gNowToken ) ) {

      if ( JudgeFloat( gNowToken ) ) {
        gIsFloat = true ;
      } // if

      string string_Num = gNowToken ;
      TakeToken() ; //  take Num

      if ( sign == "+" ) {
        return 1 * atof( string_Num.c_str() ) ;
      } // if +Num
      else if ( sign == "-" ) {
        return -1 * atof( string_Num.c_str() ) ;
      } // else if -Num

    } // if
    else {
      throw UNEXPECTED ;
    } // else
  } // if sign & Num
  else if ( JudgeNum( gNowToken ) ) {
    string string_Num = gNowToken ;
    TakeToken() ; // take Num

    if ( JudgeFloat( string_Num ) ) {
      gIsFloat = true ;
    } // if

    return atof( string_Num.c_str() ) ;
  } // else if Num without sign
  else if ( gNowToken == "(" ) {
    TakeToken() ; // take "("
    return_ans = ArithExp() ;

    if ( gNowToken.empty() ) {
      gNowToken = GetToken() ; // ")"
    } // if

    if ( gNowToken == ")" ) {
      TakeToken() ;
      return return_ans ;
    } // if
    else {
      throw UNEXPECTED ;
    } // else
  } // else if
  else {
    throw UNEXPECTED ;
  } // else

  return return_ans ;
} // NOT_ID_StartFactor()

double ArithExp() {
  double return_ans = Term() ;
  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  if ( gNowToken == "+" || gNowToken == "-" ) {

    while ( gNowToken == "+" || gNowToken == "-" ) {
      string addOrSub = gNowToken ;
      TakeToken() ; // take + or -
      double term_return = Term() ;

      if ( addOrSub == "+" ) {
        return_ans += term_return ;
      } // if '+' <Term>
      else if ( addOrSub == "-" ) {
        return_ans -= term_return ;
      } // else if '-' <Term>

      if ( gNowToken.empty() ) {
        gNowToken = GetToken() ;
      } // if

    } // while

  } // if

  return return_ans ;
} // ArithExp()

double Term() {
  double return_ans = Factor() ;

  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  if ( gNowToken == "*" || gNowToken == "/" ) {

    while ( gNowToken == "*" || gNowToken == "/" ) {
      string multiOrDiv = gNowToken ;
      TakeToken() ; // take * or /
      double factor_return = Factor() ;
      if ( multiOrDiv == "*" ) {
        return_ans *= factor_return ;

        if ( !gIsFloat ) {
          return_ans = ( int ) return_ans ;
        } // if

      } // if '*' <Factor>
      else if ( multiOrDiv == "/" ) {
        if ( factor_return == 0 ) {
          throw ERROR ;
        } // if Don't /0

        return_ans /= factor_return ;

        if ( !gIsFloat ) {
          return_ans = ( int ) return_ans ;
        } // if

      } // else if '/' <Factor>

      if ( gNowToken.empty() ) {
        gNowToken = GetToken();
      } // if
    } // while

  } // if

  return return_ans ;
} // Term()

double Factor() {

  if ( gNowToken.empty() ) {
    gNowToken = GetToken() ;
  } // if

  if ( JudgeIDENT( gNowToken ) ) {
    int ident_index = JudgeDefine( gNowToken, true ) ;
    if ( gId[ident_index].isFloat ) gIsFloat = true ;

    return gId[ident_index].identify_value ;
  } // if IDENT
  else if ( gNowToken == "+" || gNowToken == "-" ) {
    string sign = gNowToken ;
    TakeToken() ; // "+" or "-"
    if ( gNowToken.empty() ) {
      gNowToken = GetToken(); // get Num
    } // if

    if ( JudgeNum( gNowToken ) ) {

      if ( JudgeFloat( gNowToken ) ) {
        gIsFloat = true ;
      } // if

      string string_Num = gNowToken ;
      TakeToken() ; //  take Num

      if ( sign == "+" ) {
        return 1 * atof( string_Num.c_str() ) ;
      } // if +Num
      else if ( sign == "-" ) {
        return -1 * atof( string_Num.c_str() ) ;
      } // else if -Num

    } // if
    else {
      throw UNEXPECTED ;
    } // else
  } // else if sign Num
  else if ( JudgeNum( gNowToken ) ) {
    string string_Num = gNowToken ;
    TakeToken() ; //  take Num
    if ( JudgeFloat( string_Num ) ) {
      gIsFloat = true ;
    } // if

    return atof( string_Num.c_str() ) ;
  } // else if pure Num
  else if ( gNowToken == "(" ) {
    TakeToken() ; // '('
    double return_ans = ArithExp() ;

    if ( gNowToken.empty() ) {
      gNowToken = GetToken() ; // ')'
    } // if

    if ( gNowToken != ")" ) {
      throw UNEXPECTED ;
    } // if
    else {
      TakeToken() ;
      return return_ans ;
    } // else

  } // else if '(' <ArithExp> ')'
  else {
    throw UNEXPECTED ;
  } // else

  return 0.0 ;
} // Factor()
