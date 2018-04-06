# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <vector>
# include <ctype.h>
# include <iomanip>


using namespace std;

// ======================Data Struct==============================
enum Error_type{ UNRECOGNIZED, UNDEFINED, UNEXPECTED, QUIT };

struct Identify_Data{
  string identify_Name ;
  double identify_value ;
  bool isFloat ;
} ;

// ===============================================================

// ======================global var===============================
string gNowToken ;
string gPeekToken ;
vector<Identify_Data> gId ;
int gTestNum = 0 ;
int gFirstUnknow = -1 ;
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
bool StopRead( char aChar ) ;
string SpecialToken( char aChar, string Token ) ;
string ReadFloat( string Token ) ;
bool IsFloat() ;
void TakeToken() ;
void ReadAfterError() ;
bool UnknowChar( char aChar ) ;
// ===============================================================

// =======================use for judge===========================
bool JudgeIDENT( string Token ) ;
bool JudgeNum( string Token ) ;
bool JudgeFloat( string Token ) ;
bool JudgeInt( string Token ) ;
bool IsQuit( string Token ) ;
bool JudgeFactor( string Token ) ;
bool JudgeTerm( string Token ) ;
bool JudgeArithExp( string Token ) ;
bool Judge_NOT_ID_StartFactor( string Token ) ;
bool Judge_NOT_ID_StartTerm( string Token ) ;
bool Judge_NOT_ID_StartArithExp( string Token ) ;
bool Judge_NOT_ID_StartArithExpOrBexp( string Token ) ;
bool JudgeBooleanOprator( string Token ) ;
bool JudgeIDlessArithExpOrBexp( string Token ) ;
void JudgeUnrecognizedChar() ;
// ===============================================================

// =======================recursive===============================
void Command() ;
double IDlessArithExpOrBexp() ;
void BooleanOprator() ;
void NOT_ID_StartArithExpOrBexp() ;
double NOT_ID_StartArithExp() ;
double NOT_ID_StartTerm() ;
double NOT_ID_StartFactor() ;
double ArithExp() ;
double Term() ;
double Factor() ;
// ===============================================================

int main() {

  // cin >> gTestNum ;
  while ( !gQuit ) {
    try {
      Command() ;
      InitializeState() ;
    } // try
    catch ( Error_type error ) {
      ReadAfterError() ;
      if ( error == UNRECOGNIZED ) {
        cout << "Unrecognized token with first char : '" << gUnknowChar << "'" << endl ;
      } // if
      else if ( error == UNDEFINED ) {
        cout << "UNDEFINED\n" ;
      } // else if
      else if ( error == UNEXPECTED ) {
        cout << "Unexpected token : '" << gNowToken << "'" << endl ;
      } // else if
      else if ( error == QUIT ) {
        cout << "> Program exits..." ;
        return 0 ;
      } // else if
    } // catch
  } // while

} // main()

string GetToken() {
  InitializeToken() ;
  char aChar ;
  char peekChar ;
  SkipExitChar() ; // 跳過一開始空白
  string token = "" ;
  peekChar = cin.peek() ;
  if ( !StopRead( peekChar ) ) {
    while ( !StopRead( peekChar ) ) {
      aChar = cin.get() ;
      token = token + aChar ;
      peekChar = cin.peek() ;
    } // while

    if ( peekChar == '.' ) {
      token = ReadFloat( token ) ;
    } // if
  } // if
  else {
    token = SpecialToken( peekChar, token ) ;
  } // else

  return token ;

} // GetToken()

bool UnknowChar( char aChar ) {
  if ( isalnum( aChar ) || aChar == '_' || aChar == '.' || aChar == '(' || aChar == ')' ||
       aChar == '+' || aChar == '-' || aChar == '*' || aChar == '/' || aChar == '>' ||
       aChar == '<' || aChar == ':' || aChar == '=' ) {
    return false ;
  } // if
  else return true ;

  return true ;
} // UnknowChar()

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

bool StopRead( char aChar ) {

  if ( aChar == '+' || aChar == '-' || aChar == '*' || aChar == '/'
       || aChar == ';' || aChar == '>' || aChar == '<' || aChar == ':'
       || aChar == ' ' || aChar == '\n' || aChar == '\0' || aChar == '\t'
       || aChar == ')' || aChar == '(' || aChar == '.' || aChar == '=' ) {
    return true ;
  } // if
  else {
    if ( isalnum( aChar ) || aChar == '_' ) {
      return false ;
    } // if a~z || A~Z || 0~9 || _

    gUnknowChar = aChar ;
    throw UNRECOGNIZED ;
  } // else

  return false ;

} // StopRead()

void InitializeToken() {
  gNowToken.clear() ;
} // InitializeToken()

void InitializeState() {
  gNowToken.clear() ;
  gPeekToken.clear() ;
  gIsFloat = false ;
  gQuit = false ;
} // InitializeState()

string SpecialToken( char aChar, string Token ) {
  char commit[500] ; // 註解
  char peekChar ;
  if ( aChar == '+' || aChar == '-' || aChar == '*' || aChar == '/'
       || aChar == '(' || aChar == ')' ) {
    aChar = cin.get() ;
    if ( aChar == '/' ) {
      Token = Token + aChar ;
      peekChar = cin.peek() ;
      if ( peekChar == '/' ) {
        peekChar = cin.get() ;
        cin.getline( commit, 500 ) ;
        Token = GetToken() ;
      } // if 註解
    } // if
    else {
      Token = Token + aChar ;
    } // else
  } // if +-*/ && 註解
  else if ( aChar == ';' || aChar == '>' || aChar == '<' || aChar == '=' || aChar == ':' ) {
    aChar = cin.get() ;
    Token = Token + aChar ;
    if ( aChar == '>' || aChar == '<' || aChar == ':' ) {
      char peekChar ;
      peekChar = cin.peek() ;
      if ( peekChar == '=' ) {
        peekChar = cin.get() ;
        Token = Token + peekChar ;
      } // if case: >= <= :=

      if ( aChar == '<' && peekChar == '>' ) {
        peekChar = cin.get() ;
        Token = Token + peekChar ;
      } // if case : <> ( not equal )

    } // if
  } // else if boolean && ;
  else if ( aChar == '.' ) {
    aChar = cin.get() ;
    Token = Token + aChar ;
    peekChar = cin.peek() ;
    for ( int i = 1; isdigit( peekChar ) ; i++ ) {
      aChar = cin.get() ;
      Token = Token + aChar ;
      peekChar = cin.peek() ;
    } // for
  } // else if

  return Token ;

} // SpecialToken()

string ReadFloat( string Token ) {
  char aChar ;
  char peekChar ;
  aChar = cin.get() ; // read '.'
  Token = Token + aChar ;
  peekChar = cin.peek() ;
  while ( isdigit( peekChar ) ) {
    aChar = cin.get() ;
    Token = Token + aChar ;
    peekChar = cin.peek() ;
  } // while

  return Token ;
} // ReadFloat()

bool IsQuit( string Token ) {
  if ( Token == "quit" ) {
    gQuit = true ;
    return true ;
  } // if

  return false ;
} // IsQuit()

void Command() {

  gNowToken = GetToken() ;

  if ( IsQuit( gNowToken ) ) {
    throw QUIT ;
  } // if quit
  else if ( JudgeIDENT( gNowToken ) ) {
    cout << "ID" << endl ;
  } // if IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'
  else if ( !JudgeIDENT( gNowToken ) ) {
    NOT_ID_StartArithExpOrBexp() ;
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
  bool first = true ;

  if ( length == 1 && Token.at( 0 ) == '.' ) {
    return false ;
  } // if just one point '.'

  for ( int i = 0 ; i < length ; i++ ) {
    if ( Token.at( i ) >= '0' && Token.at( i ) <= '9' ) {
      correct++ ;
    } // if  0~9
    else if ( Token.at( i ) == '.' && first ) {
      correct++ ;
      first = false ;
    } // else if judge 123.111 && .2 && 2. should take care
  } // for

  if ( correct == length ) return true ;

  return false ;
} // JudgeFloat()

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

void JudgeUnrecognizedChar() {

} // JudgeUnrecognizedChar()

void NOT_ID_StartArithExpOrBexp() {
  double return_ans = NOT_ID_StartArithExp() ;

} // NOT_ID_StartArithExpOrBexp()

double IDlessArithExpOrBexp() {
  return 0 ;
} // IDlessArithExpOrBexp()

void BooleanOprator() {
} // BooleanOprator()

double NOT_ID_StartArithExp() {
  double return_ans = NOT_ID_StartTerm() ;
  return 0;
} // NOT_ID_StartArithExp()


double NOT_ID_StartTerm() {
  double return_ans = NOT_ID_StartFactor() ;
  return 0;
} // NOT_ID_StartTerm()


double NOT_ID_StartFactor() {
  double return_ans = 0.0 ;

  if ( gNowToken == "+" || gNowToken == "-" ) {
    string sign = gNowToken ;
    TakeToken() ; // take [sign]
    gNowToken = GetToken() ; // get Num
    if ( JudgeNum( gNowToken ) ) {

      if ( JudgeFloat( gNowToken ) ) {
        gIsFloat = true ;
      } // if

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

    if ( JudgeFloat( gNowToken ) ) {
      gIsFloat = true ;
    } // if

    return atof( string_Num.c_str() ) ;
  } // else if Num without sign

  return 0;
} // NOT_ID_StartFactor()


double ArithExp() {
  Term() ;
  cout << gNowToken << "\bIs ArithExp" << endl ;
  return 0;
} // ArithExp()

double Term() {
  double factor_return = Factor() ;
  cout << gNowToken << "\bIs Term" << endl ;
  return 0;
} // Term()

double Factor() {
  bool findVar = false ;
  gNowToken = GetToken() ;
  if ( JudgeIDENT( gNowToken ) ) {
    for ( int i = 0 ; i < gId.size() ; i++ ) {
      if ( gId[i].identify_Name == gNowToken ) {
        if ( gId[i].isFloat ) {
          gIsFloat = true ;
        } // if

        TakeToken() ;
        findVar = true ;
        return gId[i].identify_value ;
      } // if vector has this var
    } // for

    if ( !findVar ) {
      throw UNDEFINED ;
    } // if

  } // if IDENT
  else if ( gNowToken == "+" || gNowToken == "-" ) {
    string sign = gNowToken ;
    TakeToken() ; // "+" or "-"
    gNowToken = GetToken() ; // get Num

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

  } // else if pure Num
  else {

  } // else
  cout << gNowToken << "\bIs Factor" << endl ;
  return 0;
} // Factor()
