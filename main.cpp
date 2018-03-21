# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sstream>
# include <ctype.h>

using namespace std;

enum Error_type{ UNRECOGNIZED, UNDEFINED, UNEXPECTED, QUIT };

char gToken[50] ;
char gPeekToken[50] ;
int gTestNum = 0 ;
int gFirstUnknow = -1 ;
bool gQuit = false ;
bool gFirstIn = true ;
bool gNumIsFloat = false ;

// ======================use for Initialize======================
void InitializeToken( char Token[50] ) ;
// ===============================================================


// =======================use for get token=======================
void GetToken( char Token[50] ) ;
void SkipExitChar() ;
bool CharArrayEquals( char a [50], char b [50] ) ;
bool StopRead( char aChar ) ;
void SpecialToken( char aChar, char Token[50] ) ;
void ReadFloat( int i, char Token[50] ) ;
bool IsFloat() ;
void TakeToken() ;
void ReadAfterError() ;
bool UnknowChar( char aChar ) ;
// ===============================================================

// =======================use for judge===========================
bool JudgeIDENT( char Token[50] ) ;
bool JudgeNum( char Token[50] ) ;
bool JudgeFloat( char Token[50] ) ;
bool JudgeInt( char Token[50] ) ;
bool IsQuit( char Token[50] ) ;
bool JudgeFactor( char Token[50] ) ;
bool JudgeTerm( char Token[50] ) ;
bool JudgeArithExp( char Token[50] ) ;
bool Judge_NOT_ID_StartFactor( char Token[50] ) ;
bool Judge_NOT_ID_StartTerm( char Token[50] ) ;
bool Judge_NOT_ID_StartArithExp( char Token[50] ) ;
bool Judge_NOT_ID_StartArithExpOrBexp( char Token[50] ) ;
bool JudgeBooleanOprator( char Token[50] ) ;
bool JudgeIDlessArithExpOrBexp( char Token[50] ) ;
void JudgeUnrecognizedChar() ;
// ===============================================================

// =======================recursive===============================
void Command() ;
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
    } // try
    catch ( Error_type error ) {
      if ( error == UNRECOGNIZED ) {
        cout << "UNRECOGNIZED\n" ;
      } // if
      else if ( error == UNDEFINED ) {
        cout << "UNDEFINED\n" ;
      } // else if
      else if ( error == UNEXPECTED ) {
        cout << "UNEXPECTED\n" ;
      } // else if
      else if ( error == QUIT ) {
        cout << "> Program exits..." ;
        return 0 ;
      } // else if
    } // catch
  } // while

} // main()

void GetToken( char Token[50] ) {
  InitializeToken( Token ) ;
  char aChar ;
  char peekChar ;
  SkipExitChar() ; // 跳過一開始空白
  int i = 0 ;
  peekChar = cin.peek() ;
  if ( !StopRead( peekChar ) ) {
    while ( !StopRead( peekChar ) ) {
      scanf( "%c", &aChar ) ;
      Token[i] = aChar ;
      i++ ;
      peekChar = cin.peek() ;
    } // while

    if ( peekChar == '.' ) {
      ReadFloat( i, Token ) ;
    } // if
  } // if
  else {
    SpecialToken( peekChar, Token ) ;
  } // else
} // GetToken()

void SkipExitChar() {

  char aChar ;
  char exitChar ;
  aChar = cin.peek() ;

  if ( aChar != ' ' && aChar != '\n' && aChar != '\0' && aChar != '\t' ) {

    return ;
  } // if
  else {

    while ( aChar == ' ' || aChar == '\n' || aChar == '\0' || aChar == '\t' ) {
      scanf( "%c", &exitChar ) ;
      aChar = cin.peek() ;
    } // while


  } // else

} // SkipExitChar()

bool CharArrayEquals( char a [50], char b [50] ) {
  int as = strlen( a );
  int bs = strlen( b );
  int correct = 0 ;
  int i = 0;
  if ( as != bs ) {
    return false ;
  } // if

  for ( i = 0 ; i < as ; i++ ) {
    if ( a [i] == b [i] ) {
      correct++ ;
    } // if

  } // for

  if ( correct == as ) {
    return true ;
  } // if
  else {
    return false ;
  } // else

} // CharArrayEquals()

bool StopRead( char aChar ) {
  if ( aChar == '+' || aChar == '-' || aChar == '*' || aChar == '/'
       || aChar == ';' || aChar == '>' || aChar == '<' || aChar == ':'
       || aChar == ' ' || aChar == '\n' || aChar == '\0' || aChar == '\t'
       || aChar == ')' || aChar == '(' || aChar == '.' ) {
    return true ;
  } // if
  else {
    return false ;
  } // else

  return false ;

} // StopRead()

void InitializeToken( char Token[50] ) {
  for ( int i = 0 ; i < 50 ; i++ ) {
    Token[i] = '\0' ;
  } // for
} // InitializeToken()

void SpecialToken( char aChar, char Token[50] ) {
  char commit[100] ; // 註解
  char peekChar ;
  if ( aChar == '+' || aChar == '-' || aChar == '*' || aChar == '/'
       || aChar == '(' || aChar == ')' ) {
    scanf( "%c", &aChar ) ;
    if ( aChar == '/' ) {
      Token[0] = aChar ;
      peekChar = cin.peek() ;
      if ( peekChar == '/' ) {
        scanf( "%c", &peekChar ) ;
        for ( int i = 0 ; peekChar != '\n' ; i++ ) {
          commit[i] = peekChar ;
          scanf( "%c", &peekChar ) ;
        } // for

        GetToken( Token ) ;
      } // if 註解
    } // if
    else {
      Token[0] = aChar ;
    } // else
  } // if +-*/ && 註解
  else if ( aChar == ';' || aChar == '>' || aChar == '<' || aChar == ':' ) {
    scanf( "%c", &aChar ) ;
    Token[0] = aChar ;
    if ( aChar == '>' || aChar == '<' || aChar == ':' ) {
      char peekChar ;
      peekChar = cin.peek() ;
      if ( peekChar == '=' ) {
        scanf( "%c", &peekChar ) ;
        Token[1] = peekChar ;
      } // if
    } // if
  } // else if boolean && ;
  else if ( aChar == '.' ) {
    scanf( "%c", &aChar ) ;
    Token[0] = aChar ;
    peekChar = cin.peek() ;
    for ( int i = 1; isdigit( peekChar ) ; i++ ) {
      scanf( "%c", &aChar ) ;
      Token[i] = aChar ;
      peekChar = cin.peek() ;
    } // for
  } // else if
} // SpecialToken()

void ReadFloat( int i, char Token[50] ) {
  char aChar ;
  char peekChar ;
  scanf( "%c", &aChar ) ;
  Token[i] = aChar ;
  i++ ;
  peekChar = cin.peek() ;
  while ( isdigit( peekChar ) ) {
    scanf( "%c", &aChar ) ;
    Token[i] = aChar ;
    i++ ;
    peekChar = cin.peek() ;
  } // while

} // ReadFloat()

bool IsQuit( char Token[50] ) {
  if ( Token[0] == 'q' && Token[1] == 'u' && Token[2] == 'i' && Token[3] == 't' ) {
    gQuit = true ;
    return true ;
  } // if

  return false ;
} // IsQuit()

void Command() {

  if ( gFirstIn ) {
    GetToken( gToken ) ;
    if ( !IsQuit( gToken ) ) {
      GetToken( gPeekToken ) ;
    } // if
    else {
      throw QUIT ; // if the first input is quit
    } // else

    gFirstIn = false ;

  } // if


  if ( JudgeIDENT( gToken ) ) {
    TakeToken() ; // get the token(IDENT)
    if( strlen( gToken ) == 2 && gToken[0] == ':' && gToken[1] == '=' ) {
      TakeToken() ; // get the token(":=")

      if ( JudgeArithExp( gToken ) ) {
        ArithExp() ; // DO <ArithExp>
      }  // if

    } // if get ':=' and do <ArithExp>
    else if ( JudgeIDlessArithExpOrBexp( gToken ) ) {
      cout << "is JudgeIDlessArithExpOrBexp" << endl ;
    } // else if <IDlessArithExpOrBexp>
    else {
      throw UNRECOGNIZED ;
    } // else
  } // if IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'
  else if ( gToken[0] == '-' ||  gToken[0] == '+' || isdigit( gToken[0] )
            || gToken[0] == '.' || gToken[0] == '(' ) {
    NOT_ID_StartArithExpOrBexp() ;
  } // else if <NOT_IDStartArithExpOrBexp>
  else if ( IsQuit( gToken ) ) {
    throw QUIT ;
  } // else if QUIT
  else {
    ReadAfterError() ;
    throw ( UNEXPECTED ) ;
  } // else

} // Command()

bool JudgeIDENT( char Token[50] ) {
  int length = strlen( Token ) ;
  int correct = 0 ;
  if ( isalpha( Token[0] ) || Token[0] == '_' ) {
    correct++ ;
  } // if a~z||A~Z|| _

  for ( int i = 1 ; i < length ; i++ ) {
    if ( isalnum( Token[i] ) || Token[i] == '_' ) {
      correct++ ;
    } // if  a~z || A~Z || 0~9 || _
  } // for

  if ( correct == length ) return true ;

  return false ;

} // JudgeIDENT()

bool JudgeNum( char Token[50] ) {
  if ( JudgeFloat( Token ) || JudgeInt( Token ) ) {
    return true ;
  } // if

  return false ;
} // JudgeNum()

bool JudgeInt( char Token[50] ) {
  int length = strlen( Token ) ;
  int correct = 0 ;
  for ( int i = 0 ; i < length ; i++ ) {
    if ( Token[i] >= '0' && Token[i] <= '9' ) {
      correct++ ;
    } // if  0~9
  } // for

  if ( correct == length ) return true ;

  return false ;
} // JudgeInt

bool JudgeFloat( char Token[50] ) {
  int length = strlen( Token ) ;
  int correct = 0 ;
  bool first = true ;

  if ( length == 1 && Token[0] == '.' ) {
    return false ;
  } // if just one point '.'

  for ( int i = 0 ; i < length ; i++ ) {
    if ( Token[i] >= '0' && Token[i] <= '9' ) {
      correct++ ;
    } // if  0~9
    else if ( Token[i] == '.' && first ) {
      correct++ ;
      first = false ;
    } // else judge 123.111 && .2 && 2. should take care
  } // for

  if ( correct == length ) return true ;

  return false ;
} // JudgeFloat()

bool JudgeArithExp( char Token[50] ) {
  if ( JudgeTerm( Token ) ) return true ;

  return false ;
} // JudgeArithExp()

bool JudgeTerm( char Token[50] ) {
  if ( JudgeFactor( Token ) ) return true ;

  return false ;
} // JudgeTerm()

bool JudgeFactor( char Token[50] ) {
  if ( JudgeIDENT( Token ) ) {
    return true ;
  } // IDENT
  else if ( ( strlen( Token ) == 1 ) && ( Token[0] == '+' || Token[0] == '-' ) ) {
    if ( JudgeNum( gPeekToken ) ) {
      return true ;
    } // if case : +2 -3
    else {
      return false ;
    } // else +a -b  return error
  } // [ SIGN ] NUM
  else if ( JudgeNum( Token ) ) {
    return true ;
  } // else if Just NUM no sign case: 1 28
  else if ( ( strlen( Token ) == 1 ) && Token[0] == '(' ) {
    return true ;
  } // '(' <ArithExp> ')'
  else {
    return false ;
  } // else

  return false ;

} // JudgeFactor()

bool JudgeIDlessArithExpOrBexp( char Token[50] ) {
  return true ;
} // JudgeIDlessArithExpOrBexp()

void ReadAfterError() {
  char aChar ;
  aChar = cin.peek() ;
  while ( aChar != '\n' ) {
    scanf( "%c", &aChar ) ;
    aChar = cin.peek() ;
  } // while
} // ReadAfterError()

void TakeToken() {
  char nextToken[50] ;
  GetToken( nextToken ) ;
  for ( int i = 0 ; i < 50 ; i++ ) {
    gToken[i] = gPeekToken[i] ;
    gPeekToken[i] = nextToken[i] ;
  } // for
} // TakeToken()

void JudgeUnrecognizedChar() {

} // JudgeUnrecognizedChar()

bool UnknowChar( char aChar ) {
  if ( isalnum( aChar ) || aChar == '_' || aChar == '.' || aChar == '(' || aChar == ')' ||
       aChar == '+' || aChar == '-' || aChar == '*' || aChar == '/' || aChar == '>' ||
       aChar == '<' || aChar == ':' || aChar == '=' ) {
    return false ;
  } // if
  else return true ;

  return true ;
} // UnknowChar()

void NOT_ID_StartArithExpOrBexp() {
  cout << "NOT_ID_StartArithExpOrBexp:" << gToken << endl ;
} // NOT_ID_StartArithExpOrBexp()


double NOT_ID_StartArithExp() {
  return 0;
} // NOT_ID_StartArithExp()


double NOT_ID_StartTerm() {
  return 0;
} // NOT_ID_StartTerm()


double NOT_ID_StartFactor() {
  return 0;
} // NOT_ID_StartFactor()


double ArithExp() {
  cout << gToken <<"\bIs ArithExp" << endl ;
  return 0;
} // ArithExp()

double Term() {
  cout << gToken <<"\bIs Term" << endl ;
  return 0;
} // Term()

double Factor() {
  cout << gToken <<"\bIs Factor" << endl ;
  return 0;
} // Factor()
