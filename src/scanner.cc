//************************************************************************************
// CPSC 4600
// PL Language Scanner
// Scanner Implementation File
// Written By: Eric Den Haan and Blake Nelson
//************************************************************************************

#include "./scanner.h"

// Constructors
Scanner::Scanner(ifstream &inputFile, SymbolTable &table)
{
  SymbolTable t = table;
  inputFilePtr = &inputFile;
  symbolTablePtr = &table;
  symbolTablePtr->init();
}

// Assignment Operator
Scanner Scanner::operator=(const Scanner &s)
{
  this->inputFilePtr = s.inputFilePtr;
  this->symbolTablePtr = s.symbolTablePtr;
  this->nextChar = s.nextChar;

  return *this;
}

// Destructor
Scanner::~Scanner()
{
  inputFilePtr = nullptr;
  symbolTablePtr = nullptr;
}

// getSymbolTablePtr Method
SymbolTable *Scanner::getSymbolTablePtr()
{
  return symbolTablePtr;
}

// isWhitespace Method
bool Scanner::isWhitespace(const char &c)
{
  if (isspace(c))
  {
    return true;
  }
  return false;
}

// isAlpha Method
bool Scanner::isAlpha(const char &c)
{
  if (isalpha(c))
  {
    return true;
  }
  return false;
}

// isDigit Method
bool Scanner::isDigit(const char &c)
{
  if (isdigit(c))
  {
    return true;
  }
  return false;
}

// isSpecial Method
bool Scanner::isSpecial(const char &c)
{
  char specials[19] = {
      '.',
      ',',
      ';',
      '~',
      '<',
      '>',
      '=',
      '[',
      ']',
      '&',
      '|',
      '+',
      '-',
      '*',
      '\\',
      '/',
      '(',
      ')',
      ':',
  };
  if (find(begin(specials), end(specials), c) != end(specials))
  {
    return true;
  }
  return false;
}

//getToken Method
Token Scanner::getToken()
{
  nextChar = inputFilePtr->get();
  Token t;

  if (isWhitespace(nextChar))
  {
    //
  }

  else if (isAlpha(nextChar))
  {
    t = recognizeId();
  }

  else if (isDigit(nextChar))
  {
    t = recognizeDigit();
  }

  else if (isSpecial(nextChar))
  {
    t = recognizeSpecial();
  }

  else if (nextChar == '$')
  {
    recognizeComment();
  }

  else
  {
    t = Token();
  }

  return t;
}

// recognizeId Method
Token Scanner::recognizeId()
{
  string lexeme(1, nextChar);
  char temp = inputFilePtr->peek();
  while (!isWhitespace(temp) && (temp == '_' || isAlpha(temp) || isDigit(temp)))
  {
    string nextPart(1, temp);
    lexeme += nextPart;
    nextChar = inputFilePtr->get();
    temp = inputFilePtr->peek();
  }
  string lexemeSigChars = lexeme.substr(0, 10);

  int index = symbolTablePtr->search(lexemeSigChars);
  if (index == -1)
  {
    Token t = Token(ID, Attribute(lexemeSigChars));
    // cout << "IDENTIFIER: " << lexemeSigChars << endl;
    return t;
  }
  return Token();
}

// recognizeDigit Method
Token Scanner::recognizeDigit()
{
  string digitString(1, nextChar);
  char temp = inputFilePtr->peek();
  while (isDigit(temp))
  {
    string nextPart(1, temp);
    digitString += nextPart;
    nextChar = inputFilePtr->get();
    temp = inputFilePtr->peek();
  }

  int digit = stoi(digitString);
  Token t = Token(NUM, Attribute(digit));
  // cout << "DIGIT: " << digit << endl;
  return t;
}

// recognizeSpecial Method
Token Scanner::recognizeSpecial()
{
  char special = nextChar;
  if (special == ':')
  {
    char temp = inputFilePtr->peek();
    if (temp == '=')
    {
      nextChar = inputFilePtr->get();
      Token t = Token(ASSIGNMENT_OPERATOR, Attribute());
      // cout << "SPECIAL: " << special << temp << endl;
      return t;
    }
    // Throw error
  }
  if (special == '[')
  {
    char temp = inputFilePtr->peek();
    if (temp == ']')
    {
      nextChar = inputFilePtr->get();
      Token t = Token(GUARDED_COMMAND, Attribute());
      // cout << "SPECIAL: " << special << temp << endl;
      return t;
    }
  }
  if (special == '-')
  {
    char temp = inputFilePtr->peek();
    if (temp == '>')
    {
      nextChar = inputFilePtr->get();
      Token t = Token(ARROW, Attribute());
      // cout << "SPECIAL: " << special << temp << endl;
      return t;
    }
  }
  Symbol s;
  if (special == '.')
  {
    s = PERIOD;
  }
  else if (special == ',')
  {
    s = COMMA;
  }
  else if (special == ';')
  {
    s = SEMICOLON;
  }
  else if (special == '~')
  {
    s = NOT_OPERATOR;
  }
  else if (special == '<')
  {
    s = LESS_THAN_OPERATOR;
  }
  else if (special == '>')
  {
    s = GREATER_THAN_OPERATOR;
  }
  else if (special == '=')
  {
    s = EQUAL_OPERATOR;
  }
  else if (special == '[')
  {
    s = OPEN_SQUARE_BRACKET;
  }
  else if (special == ']')
  {
    s = CLOSE_SQUARE_BRACKET;
  }
  else if (special == '&')
  {
    s = AND_OPERATOR;
  }
  else if (special == '|')
  {
    s = OR_OPERATOR;
  }
  else if (special == '+')
  {
    s = ADDITION_OPERATOR;
  }
  else if (special == '-')
  {
    s = SUBTRACTION_OPERATOR;
  }
  else if (special == '*')
  {
    s = MULTIPLICATION_OPERATOR;
  }
  else if (special == '\\')
  {
    s = MODULUS_OPERATOR;
  }
  else if (special == '/')
  {
    s = DIVISION_OPERATOR;
  }
  else if (special == '(')
  {
    s = OPEN_PARENTHESIS;
  }
  else if (special == ')')
  {
    s = CLOSE_PARENTHESIS;
  }

  // cout << "SPECIAL: " << special << endl;
  Token t = Token(s, Attribute());
  return t;
}

void Scanner::recognizeComment()
{
  char comment[1028];
  inputFilePtr->getline(comment, 1028);
  // cout << "COMMENT: " << comment << endl;
}
