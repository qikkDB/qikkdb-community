
// Generated from C:/GPU-DB/dropdbase/GpuSqlParser\GpuSqlLexer.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  GpuSqlLexer : public antlr4::Lexer {
public:
  enum {
    DATETIMELIT = 1, LF = 2, CR = 3, CRLF = 4, WS = 5, SEMICOL = 6, SQOUTE = 7, 
    DQOUTE = 8, UNDERSCORE = 9, COLON = 10, COMMA = 11, DOT = 12, LSQR_BRC = 13, 
    RSQR_BRC = 14, STRING = 15, DELIMID = 16, DATELIT = 17, TIMELIT = 18, 
    POINT = 19, MULTIPOINT = 20, LINESTRING = 21, MULTILINESTRING = 22, 
    POLYGON = 23, MULTIPOLYGON = 24, INTTYPE = 25, LONGTYPE = 26, DATETYPE = 27, 
    DETETIMETYPE = 28, FLOATTYPE = 29, DOUBLETYPE = 30, STRINGTYPE = 31, 
    BOOLEANTYPE = 32, POINTTYPE = 33, POLYTYPE = 34, INSERTINTO = 35, CREATEDB = 36, 
    DROPDB = 37, CREATETABLE = 38, DROPTABLE = 39, ALTERTABLE = 40, ALTERDATABASE = 41, 
    DROPCOLUMN = 42, ALTERCOLUMN = 43, RENAMECOLUMN = 44, RENAMETO = 45, 
    CREATEINDEX = 46, INDEX = 47, UNIQUE = 48, PRIMARYKEY = 49, CREATE = 50, 
    ADD = 51, DROP = 52, ALTER = 53, RENAME = 54, SET = 55, DATABASE = 56, 
    TABLE = 57, COLUMN = 58, BLOCKSIZE = 59, VALUES = 60, SELECT = 61, FROM = 62, 
    JOIN = 63, WHERE = 64, GROUPBY = 65, AS = 66, IN = 67, TO = 68, ISNULL = 69, 
    ISNOTNULL = 70, NOTNULL = 71, BETWEEN = 72, ON = 73, ORDERBY = 74, DIR = 75, 
    LIMIT = 76, OFFSET = 77, INNER = 78, FULLOUTER = 79, SHOWDB = 80, SHOWTB = 81, 
    SHOWCL = 82, SHOWQTYPES = 83, SHOWCONSTRAINTS = 84, AVG_AGG = 85, SUM_AGG = 86, 
    MIN_AGG = 87, MAX_AGG = 88, COUNT_AGG = 89, YEAR = 90, MONTH = 91, DAY = 92, 
    HOUR = 93, MINUTE = 94, SECOND = 95, NOW = 96, PI = 97, ABS = 98, SIN = 99, 
    COS = 100, TAN = 101, COT = 102, ASIN = 103, ACOS = 104, ATAN = 105, 
    ATAN2 = 106, LOG10 = 107, LOG = 108, EXP = 109, POW = 110, SQRT = 111, 
    SQUARE = 112, SIGN = 113, ROOT = 114, ROUND = 115, CEIL = 116, FLOOR = 117, 
    LTRIM = 118, RTRIM = 119, LOWER = 120, UPPER = 121, REVERSE = 122, LEN = 123, 
    LEFT = 124, RIGHT = 125, CONCAT = 126, CAST = 127, RETPAYLOAD = 128, 
    GEO_CONTAINS = 129, GEO_INTERSECT = 130, GEO_UNION = 131, GEO_LONGITUDE_TO_TILE_X = 132, 
    GEO_LATITUDE_TO_TILE_Y = 133, GEO_TILE_X_TO_LONGITUDE = 134, GEO_TILE_Y_TO_LATITUDE = 135, 
    PLUS = 136, MINUS = 137, ASTERISK = 138, DIVISION = 139, MODULO = 140, 
    XOR = 141, EQUALS = 142, NOTEQUALS = 143, NOTEQUALS_GT_LT = 144, LPAREN = 145, 
    RPAREN = 146, GREATER = 147, LESS = 148, GREATEREQ = 149, LESSEQ = 150, 
    LOGICAL_NOT = 151, OR = 152, AND = 153, BIT_OR = 154, BIT_AND = 155, 
    L_SHIFT = 156, R_SHIFT = 157, BOOLEANLIT = 158, TRUE = 159, FALSE = 160, 
    FLOATLIT = 161, INTLIT = 162, NULLLIT = 163, ID = 164
  };

  GpuSqlLexer(antlr4::CharStream *input);
  ~GpuSqlLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

