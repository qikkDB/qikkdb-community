
// Generated from C:/Users/AndrejFusekInstarea/Documents/GPU-DB/qikkDB/GpuSqlParser\GpuSqlParser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  GpuSqlParser : public antlr4::Parser {
public:
  enum {
    DATETIMELIT = 1, LF = 2, CR = 3, CRLF = 4, WS = 5, SEMICOL = 6, SQOUTE = 7, 
    DQOUTE = 8, UNDERSCORE = 9, COLON = 10, COMMA = 11, DOT = 12, LSQR_BRC = 13, 
    RSQR_BRC = 14, STRING = 15, DELIMID = 16, DATELIT = 17, TIMELIT = 18, 
    POINT = 19, MULTIPOINT = 20, LINESTRING = 21, MULTILINESTRING = 22, 
    POLYGON = 23, MULTIPOLYGON = 24, INTTYPE = 25, LONGTYPE = 26, DATETYPE = 27, 
    DETETIMETYPE = 28, FLOATTYPE = 29, DOUBLETYPE = 30, STRINGTYPE = 31, 
    BOOLEANTYPE = 32, POINTTYPE = 33, POLYTYPE = 34, TYPES = 35, INDEX = 36, 
    UNIQUE = 37, PRIMARY = 38, KEY = 39, CREATE = 40, ADD = 41, DROP = 42, 
    ALTER = 43, RENAME = 44, SET = 45, INSERT = 46, INTO = 47, SHOW = 48, 
    SIZE = 49, FULL = 50, OUTER = 51, INNER = 52, DATABASE = 53, DATABASES = 54, 
    TABLE = 55, TABLES = 56, COLUMN = 57, COLUMNS = 58, BLOCK = 59, CONSTRAINTS = 60, 
    VALUES = 61, SELECT = 62, FROM = 63, JOIN = 64, WHERE = 65, GROUP = 66, 
    AS = 67, IN = 68, TO = 69, IS = 70, NOT = 71, NULL_T = 72, BY = 73, 
    BETWEEN = 74, ON = 75, ORDER = 76, DIR = 77, LIMIT = 78, OFFSET = 79, 
    QUERY = 80, AVG_AGG = 81, SUM_AGG = 82, MIN_AGG = 83, MAX_AGG = 84, 
    COUNT_AGG = 85, YEAR = 86, MONTH = 87, DAY = 88, HOUR = 89, MINUTE = 90, 
    SECOND = 91, WEEKDAY = 92, DAYOFWEEK = 93, NOW = 94, PI = 95, ABS = 96, 
    SIN = 97, COS = 98, TAN = 99, COT = 100, ASIN = 101, ACOS = 102, ATAN = 103, 
    ATAN2 = 104, LOG10 = 105, LOG = 106, EXP = 107, POW = 108, SQRT = 109, 
    SQUARE = 110, SIGN = 111, ROOT = 112, ROUND = 113, CEIL = 114, FLOOR = 115, 
    LTRIM = 116, RTRIM = 117, LOWER = 118, UPPER = 119, REVERSE = 120, LEN = 121, 
    LEFT = 122, RIGHT = 123, CONCAT = 124, CAST = 125, RETPAYLOAD = 126, 
    GEO_CONTAINS = 127, GEO_INTERSECT = 128, GEO_UNION = 129, GEO_LONGITUDE_TO_TILE_X = 130, 
    GEO_LATITUDE_TO_TILE_Y = 131, GEO_TILE_X_TO_LONGITUDE = 132, GEO_TILE_Y_TO_LATITUDE = 133, 
    PLUS = 134, MINUS = 135, ASTERISK = 136, DIVISION = 137, MODULO = 138, 
    XOR = 139, EQUALS = 140, NOTEQUALS = 141, NOTEQUALS_GT_LT = 142, LPAREN = 143, 
    RPAREN = 144, GREATER = 145, LESS = 146, GREATEREQ = 147, LESSEQ = 148, 
    LOGICAL_NOT = 149, OR = 150, AND = 151, BIT_OR = 152, BIT_AND = 153, 
    L_SHIFT = 154, R_SHIFT = 155, BOOLEANLIT = 156, TRUE = 157, FALSE = 158, 
    FLOATLIT = 159, INTLIT = 160, ID = 161
  };

  enum {
    RuleSqlFile = 0, RuleStatement = 1, RuleShowStatement = 2, RuleShowDatabases = 3, 
    RuleShowTables = 4, RuleShowColumns = 5, RuleShowConstraints = 6, RuleShowQueryTypes = 7, 
    RuleSqlSelect = 8, RuleSqlCreateDb = 9, RuleSqlDropDb = 10, RuleSqlCreateTable = 11, 
    RuleSqlDropTable = 12, RuleSqlAlterTable = 13, RuleSqlAlterDatabase = 14, 
    RuleSqlCreateIndex = 15, RuleSqlInsertInto = 16, RuleNewTableEntries = 17, 
    RuleNewTableEntry = 18, RuleAlterDatabaseEntries = 19, RuleAlterDatabaseEntry = 20, 
    RuleRenameDatabase = 21, RuleAlterTableEntries = 22, RuleAlterTableEntry = 23, 
    RuleAddColumn = 24, RuleDropColumn = 25, RuleAlterColumn = 26, RuleRenameColumn = 27, 
    RuleRenameTable = 28, RuleAddConstraint = 29, RuleDropConstraint = 30, 
    RuleAlterBlockSize = 31, RuleRenameColumnFrom = 32, RuleRenameColumnTo = 33, 
    RuleNewTableColumn = 34, RuleNewTableConstraint = 35, RuleSelectColumns = 36, 
    RuleSelectColumn = 37, RuleSelectAllColumns = 38, RuleWhereClause = 39, 
    RuleOrderByColumns = 40, RuleOrderByColumn = 41, RuleInsertIntoValues = 42, 
    RuleInsertIntoColumns = 43, RuleIndexColumns = 44, RuleConstraintColumns = 45, 
    RuleGroupByColumns = 46, RuleGroupByColumn = 47, RuleFromTables = 48, 
    RuleJoinClauses = 49, RuleJoinClause = 50, RuleJoinTable = 51, RuleJoinColumnLeft = 52, 
    RuleJoinColumnRight = 53, RuleJoinOperator = 54, RuleJoinType = 55, 
    RuleFromTable = 56, RuleColumnId = 57, RuleTable = 58, RuleColumn = 59, 
    RuleDatabase = 60, RuleAlias = 61, RuleIndexName = 62, RuleConstraintName = 63, 
    RuleLimit = 64, RuleOffset = 65, RuleBlockSize = 66, RuleColumnValue = 67, 
    RuleConstraint = 68, RuleRetpayload = 69, RuleExpression = 70, RuleDatatype = 71, 
    RuleGeometry = 72, RulePointGeometry = 73, RuleLineStringGeometry = 74, 
    RulePolygonGeometry = 75, RuleMultiPointGeometry = 76, RuleMultiLineStringGeometry = 77, 
    RuleMultiPolygonGeometry = 78, RulePointOrClosedPoint = 79, RulePolygon = 80, 
    RuleLineString = 81, RulePoint = 82
  };

  GpuSqlParser(antlr4::TokenStream *input);
  ~GpuSqlParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class SqlFileContext;
  class StatementContext;
  class ShowStatementContext;
  class ShowDatabasesContext;
  class ShowTablesContext;
  class ShowColumnsContext;
  class ShowConstraintsContext;
  class ShowQueryTypesContext;
  class SqlSelectContext;
  class SqlCreateDbContext;
  class SqlDropDbContext;
  class SqlCreateTableContext;
  class SqlDropTableContext;
  class SqlAlterTableContext;
  class SqlAlterDatabaseContext;
  class SqlCreateIndexContext;
  class SqlInsertIntoContext;
  class NewTableEntriesContext;
  class NewTableEntryContext;
  class AlterDatabaseEntriesContext;
  class AlterDatabaseEntryContext;
  class RenameDatabaseContext;
  class AlterTableEntriesContext;
  class AlterTableEntryContext;
  class AddColumnContext;
  class DropColumnContext;
  class AlterColumnContext;
  class RenameColumnContext;
  class RenameTableContext;
  class AddConstraintContext;
  class DropConstraintContext;
  class AlterBlockSizeContext;
  class RenameColumnFromContext;
  class RenameColumnToContext;
  class NewTableColumnContext;
  class NewTableConstraintContext;
  class SelectColumnsContext;
  class SelectColumnContext;
  class SelectAllColumnsContext;
  class WhereClauseContext;
  class OrderByColumnsContext;
  class OrderByColumnContext;
  class InsertIntoValuesContext;
  class InsertIntoColumnsContext;
  class IndexColumnsContext;
  class ConstraintColumnsContext;
  class GroupByColumnsContext;
  class GroupByColumnContext;
  class FromTablesContext;
  class JoinClausesContext;
  class JoinClauseContext;
  class JoinTableContext;
  class JoinColumnLeftContext;
  class JoinColumnRightContext;
  class JoinOperatorContext;
  class JoinTypeContext;
  class FromTableContext;
  class ColumnIdContext;
  class TableContext;
  class ColumnContext;
  class DatabaseContext;
  class AliasContext;
  class IndexNameContext;
  class ConstraintNameContext;
  class LimitContext;
  class OffsetContext;
  class BlockSizeContext;
  class ColumnValueContext;
  class ConstraintContext;
  class RetpayloadContext;
  class ExpressionContext;
  class DatatypeContext;
  class GeometryContext;
  class PointGeometryContext;
  class LineStringGeometryContext;
  class PolygonGeometryContext;
  class MultiPointGeometryContext;
  class MultiLineStringGeometryContext;
  class MultiPolygonGeometryContext;
  class PointOrClosedPointContext;
  class PolygonContext;
  class LineStringContext;
  class PointContext; 

  class  SqlFileContext : public antlr4::ParserRuleContext {
  public:
    SqlFileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlFileContext* sqlFile();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SqlSelectContext *sqlSelect();
    SqlCreateDbContext *sqlCreateDb();
    SqlDropDbContext *sqlDropDb();
    SqlCreateTableContext *sqlCreateTable();
    SqlDropTableContext *sqlDropTable();
    SqlAlterTableContext *sqlAlterTable();
    SqlAlterDatabaseContext *sqlAlterDatabase();
    SqlCreateIndexContext *sqlCreateIndex();
    SqlInsertIntoContext *sqlInsertInto();
    ShowStatementContext *showStatement();
    ShowQueryTypesContext *showQueryTypes();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  StatementContext* statement();

  class  ShowStatementContext : public antlr4::ParserRuleContext {
  public:
    ShowStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ShowDatabasesContext *showDatabases();
    ShowTablesContext *showTables();
    ShowColumnsContext *showColumns();
    ShowConstraintsContext *showConstraints();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowStatementContext* showStatement();

  class  ShowDatabasesContext : public antlr4::ParserRuleContext {
  public:
    ShowDatabasesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOW();
    antlr4::tree::TerminalNode *DATABASES();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowDatabasesContext* showDatabases();

  class  ShowTablesContext : public antlr4::ParserRuleContext {
  public:
    ShowTablesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOW();
    antlr4::tree::TerminalNode *TABLES();
    antlr4::tree::TerminalNode *SEMICOL();
    DatabaseContext *database();
    antlr4::tree::TerminalNode *FROM();
    antlr4::tree::TerminalNode *IN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowTablesContext* showTables();

  class  ShowColumnsContext : public antlr4::ParserRuleContext {
  public:
    ShowColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOW();
    antlr4::tree::TerminalNode *COLUMNS();
    TableContext *table();
    antlr4::tree::TerminalNode *SEMICOL();
    std::vector<antlr4::tree::TerminalNode *> FROM();
    antlr4::tree::TerminalNode* FROM(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IN();
    antlr4::tree::TerminalNode* IN(size_t i);
    DatabaseContext *database();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowColumnsContext* showColumns();

  class  ShowConstraintsContext : public antlr4::ParserRuleContext {
  public:
    ShowConstraintsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOW();
    antlr4::tree::TerminalNode *CONSTRAINTS();
    TableContext *table();
    antlr4::tree::TerminalNode *SEMICOL();
    std::vector<antlr4::tree::TerminalNode *> FROM();
    antlr4::tree::TerminalNode* FROM(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IN();
    antlr4::tree::TerminalNode* IN(size_t i);
    DatabaseContext *database();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowConstraintsContext* showConstraints();

  class  ShowQueryTypesContext : public antlr4::ParserRuleContext {
  public:
    ShowQueryTypesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOW();
    antlr4::tree::TerminalNode *QUERY();
    antlr4::tree::TerminalNode *COLUMN();
    antlr4::tree::TerminalNode *TYPES();
    SqlSelectContext *sqlSelect();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowQueryTypesContext* showQueryTypes();

  class  SqlSelectContext : public antlr4::ParserRuleContext {
  public:
    SqlSelectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SELECT();
    SelectColumnsContext *selectColumns();
    antlr4::tree::TerminalNode *FROM();
    FromTablesContext *fromTables();
    antlr4::tree::TerminalNode *SEMICOL();
    JoinClausesContext *joinClauses();
    antlr4::tree::TerminalNode *WHERE();
    WhereClauseContext *whereClause();
    antlr4::tree::TerminalNode *GROUP();
    std::vector<antlr4::tree::TerminalNode *> BY();
    antlr4::tree::TerminalNode* BY(size_t i);
    GroupByColumnsContext *groupByColumns();
    antlr4::tree::TerminalNode *ORDER();
    OrderByColumnsContext *orderByColumns();
    antlr4::tree::TerminalNode *LIMIT();
    LimitContext *limit();
    antlr4::tree::TerminalNode *OFFSET();
    OffsetContext *offset();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlSelectContext* sqlSelect();

  class  SqlCreateDbContext : public antlr4::ParserRuleContext {
  public:
    SqlCreateDbContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *DATABASE();
    DatabaseContext *database();
    antlr4::tree::TerminalNode *SEMICOL();
    BlockSizeContext *blockSize();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlCreateDbContext* sqlCreateDb();

  class  SqlDropDbContext : public antlr4::ParserRuleContext {
  public:
    SqlDropDbContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *DATABASE();
    DatabaseContext *database();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlDropDbContext* sqlDropDb();

  class  SqlCreateTableContext : public antlr4::ParserRuleContext {
  public:
    SqlCreateTableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *TABLE();
    TableContext *table();
    antlr4::tree::TerminalNode *LPAREN();
    NewTableEntriesContext *newTableEntries();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SEMICOL();
    BlockSizeContext *blockSize();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlCreateTableContext* sqlCreateTable();

  class  SqlDropTableContext : public antlr4::ParserRuleContext {
  public:
    SqlDropTableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *TABLE();
    TableContext *table();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlDropTableContext* sqlDropTable();

  class  SqlAlterTableContext : public antlr4::ParserRuleContext {
  public:
    SqlAlterTableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *TABLE();
    TableContext *table();
    AlterTableEntriesContext *alterTableEntries();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlAlterTableContext* sqlAlterTable();

  class  SqlAlterDatabaseContext : public antlr4::ParserRuleContext {
  public:
    SqlAlterDatabaseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *DATABASE();
    DatabaseContext *database();
    AlterDatabaseEntriesContext *alterDatabaseEntries();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlAlterDatabaseContext* sqlAlterDatabase();

  class  SqlCreateIndexContext : public antlr4::ParserRuleContext {
  public:
    SqlCreateIndexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *INDEX();
    IndexNameContext *indexName();
    antlr4::tree::TerminalNode *ON();
    TableContext *table();
    antlr4::tree::TerminalNode *LPAREN();
    IndexColumnsContext *indexColumns();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlCreateIndexContext* sqlCreateIndex();

  class  SqlInsertIntoContext : public antlr4::ParserRuleContext {
  public:
    SqlInsertIntoContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INSERT();
    antlr4::tree::TerminalNode *INTO();
    TableContext *table();
    std::vector<antlr4::tree::TerminalNode *> LPAREN();
    antlr4::tree::TerminalNode* LPAREN(size_t i);
    InsertIntoColumnsContext *insertIntoColumns();
    std::vector<antlr4::tree::TerminalNode *> RPAREN();
    antlr4::tree::TerminalNode* RPAREN(size_t i);
    antlr4::tree::TerminalNode *VALUES();
    InsertIntoValuesContext *insertIntoValues();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlInsertIntoContext* sqlInsertInto();

  class  NewTableEntriesContext : public antlr4::ParserRuleContext {
  public:
    NewTableEntriesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NewTableEntryContext *> newTableEntry();
    NewTableEntryContext* newTableEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NewTableEntriesContext* newTableEntries();

  class  NewTableEntryContext : public antlr4::ParserRuleContext {
  public:
    NewTableEntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NewTableColumnContext *newTableColumn();
    NewTableConstraintContext *newTableConstraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NewTableEntryContext* newTableEntry();

  class  AlterDatabaseEntriesContext : public antlr4::ParserRuleContext {
  public:
    AlterDatabaseEntriesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AlterDatabaseEntryContext *> alterDatabaseEntry();
    AlterDatabaseEntryContext* alterDatabaseEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterDatabaseEntriesContext* alterDatabaseEntries();

  class  AlterDatabaseEntryContext : public antlr4::ParserRuleContext {
  public:
    AlterDatabaseEntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RenameDatabaseContext *renameDatabase();
    AlterBlockSizeContext *alterBlockSize();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterDatabaseEntryContext* alterDatabaseEntry();

  class  RenameDatabaseContext : public antlr4::ParserRuleContext {
  public:
    RenameDatabaseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RENAME();
    antlr4::tree::TerminalNode *TO();
    DatabaseContext *database();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RenameDatabaseContext* renameDatabase();

  class  AlterTableEntriesContext : public antlr4::ParserRuleContext {
  public:
    AlterTableEntriesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AlterTableEntryContext *> alterTableEntry();
    AlterTableEntryContext* alterTableEntry(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterTableEntriesContext* alterTableEntries();

  class  AlterTableEntryContext : public antlr4::ParserRuleContext {
  public:
    AlterTableEntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AddColumnContext *addColumn();
    DropColumnContext *dropColumn();
    AlterColumnContext *alterColumn();
    RenameColumnContext *renameColumn();
    RenameTableContext *renameTable();
    AddConstraintContext *addConstraint();
    DropConstraintContext *dropConstraint();
    AlterBlockSizeContext *alterBlockSize();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterTableEntryContext* alterTableEntry();

  class  AddColumnContext : public antlr4::ParserRuleContext {
  public:
    AddColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ADD();
    ColumnContext *column();
    DatatypeContext *datatype();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AddColumnContext* addColumn();

  class  DropColumnContext : public antlr4::ParserRuleContext {
  public:
    DropColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *COLUMN();
    ColumnContext *column();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DropColumnContext* dropColumn();

  class  AlterColumnContext : public antlr4::ParserRuleContext {
  public:
    AlterColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *COLUMN();
    ColumnContext *column();
    DatatypeContext *datatype();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterColumnContext* alterColumn();

  class  RenameColumnContext : public antlr4::ParserRuleContext {
  public:
    RenameColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RENAME();
    antlr4::tree::TerminalNode *COLUMN();
    RenameColumnFromContext *renameColumnFrom();
    antlr4::tree::TerminalNode *TO();
    RenameColumnToContext *renameColumnTo();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RenameColumnContext* renameColumn();

  class  RenameTableContext : public antlr4::ParserRuleContext {
  public:
    RenameTableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RENAME();
    antlr4::tree::TerminalNode *TO();
    TableContext *table();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RenameTableContext* renameTable();

  class  AddConstraintContext : public antlr4::ParserRuleContext {
  public:
    AddConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ADD();
    ConstraintContext *constraint();
    ConstraintNameContext *constraintName();
    antlr4::tree::TerminalNode *LPAREN();
    ConstraintColumnsContext *constraintColumns();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AddConstraintContext* addConstraint();

  class  DropConstraintContext : public antlr4::ParserRuleContext {
  public:
    DropConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    ConstraintContext *constraint();
    ConstraintNameContext *constraintName();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DropConstraintContext* dropConstraint();

  class  AlterBlockSizeContext : public antlr4::ParserRuleContext {
  public:
    AlterBlockSizeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTER();
    antlr4::tree::TerminalNode *BLOCK();
    antlr4::tree::TerminalNode *SIZE();
    BlockSizeContext *blockSize();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterBlockSizeContext* alterBlockSize();

  class  RenameColumnFromContext : public antlr4::ParserRuleContext {
  public:
    RenameColumnFromContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnContext *column();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RenameColumnFromContext* renameColumnFrom();

  class  RenameColumnToContext : public antlr4::ParserRuleContext {
  public:
    RenameColumnToContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnContext *column();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RenameColumnToContext* renameColumnTo();

  class  NewTableColumnContext : public antlr4::ParserRuleContext {
  public:
    NewTableColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnContext *column();
    DatatypeContext *datatype();
    ConstraintContext *constraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NewTableColumnContext* newTableColumn();

  class  NewTableConstraintContext : public antlr4::ParserRuleContext {
  public:
    NewTableConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstraintContext *constraint();
    ConstraintNameContext *constraintName();
    antlr4::tree::TerminalNode *LPAREN();
    ConstraintColumnsContext *constraintColumns();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NewTableConstraintContext* newTableConstraint();

  class  SelectColumnsContext : public antlr4::ParserRuleContext {
  public:
    SelectColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SelectColumnContext *> selectColumn();
    SelectColumnContext* selectColumn(size_t i);
    std::vector<SelectAllColumnsContext *> selectAllColumns();
    SelectAllColumnsContext* selectAllColumns(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SelectColumnsContext* selectColumns();

  class  SelectColumnContext : public antlr4::ParserRuleContext {
  public:
    SelectColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    AliasContext *alias();
    antlr4::tree::TerminalNode *RETPAYLOAD();
    RetpayloadContext *retpayload();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SelectColumnContext* selectColumn();

  class  SelectAllColumnsContext : public antlr4::ParserRuleContext {
  public:
    SelectAllColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASTERISK();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SelectAllColumnsContext* selectAllColumns();

  class  WhereClauseContext : public antlr4::ParserRuleContext {
  public:
    WhereClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  WhereClauseContext* whereClause();

  class  OrderByColumnsContext : public antlr4::ParserRuleContext {
  public:
    OrderByColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<OrderByColumnContext *> orderByColumn();
    OrderByColumnContext* orderByColumn(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OrderByColumnsContext* orderByColumns();

  class  OrderByColumnContext : public antlr4::ParserRuleContext {
  public:
    OrderByColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *DIR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OrderByColumnContext* orderByColumn();

  class  InsertIntoValuesContext : public antlr4::ParserRuleContext {
  public:
    InsertIntoValuesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnValueContext *> columnValue();
    ColumnValueContext* columnValue(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  InsertIntoValuesContext* insertIntoValues();

  class  InsertIntoColumnsContext : public antlr4::ParserRuleContext {
  public:
    InsertIntoColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnIdContext *> columnId();
    ColumnIdContext* columnId(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  InsertIntoColumnsContext* insertIntoColumns();

  class  IndexColumnsContext : public antlr4::ParserRuleContext {
  public:
    IndexColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnContext *> column();
    ColumnContext* column(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IndexColumnsContext* indexColumns();

  class  ConstraintColumnsContext : public antlr4::ParserRuleContext {
  public:
    ConstraintColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnContext *> column();
    ColumnContext* column(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstraintColumnsContext* constraintColumns();

  class  GroupByColumnsContext : public antlr4::ParserRuleContext {
  public:
    GroupByColumnsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupByColumnContext *> groupByColumn();
    GroupByColumnContext* groupByColumn(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GroupByColumnsContext* groupByColumns();

  class  GroupByColumnContext : public antlr4::ParserRuleContext {
  public:
    GroupByColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GroupByColumnContext* groupByColumn();

  class  FromTablesContext : public antlr4::ParserRuleContext {
  public:
    FromTablesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FromTableContext *> fromTable();
    FromTableContext* fromTable(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FromTablesContext* fromTables();

  class  JoinClausesContext : public antlr4::ParserRuleContext {
  public:
    JoinClausesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<JoinClauseContext *> joinClause();
    JoinClauseContext* joinClause(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JoinClausesContext* joinClauses();

  class  JoinClauseContext : public antlr4::ParserRuleContext {
  public:
    JoinClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *JOIN();
    JoinTableContext *joinTable();
    antlr4::tree::TerminalNode *ON();
    JoinColumnLeftContext *joinColumnLeft();
    JoinOperatorContext *joinOperator();
    JoinColumnRightContext *joinColumnRight();
    JoinTypeContext *joinType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JoinClauseContext* joinClause();

  class  JoinTableContext : public antlr4::ParserRuleContext {
  public:
    JoinTableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableContext *table();
    antlr4::tree::TerminalNode *AS();
    AliasContext *alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JoinTableContext* joinTable();

  class  JoinColumnLeftContext : public antlr4::ParserRuleContext {
  public:
    JoinColumnLeftContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnIdContext *columnId();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JoinColumnLeftContext* joinColumnLeft();

  class  JoinColumnRightContext : public antlr4::ParserRuleContext {
  public:
    JoinColumnRightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnIdContext *columnId();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JoinColumnRightContext* joinColumnRight();

  class  JoinOperatorContext : public antlr4::ParserRuleContext {
  public:
    JoinOperatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATEREQ();
    antlr4::tree::TerminalNode *LESSEQ();
    antlr4::tree::TerminalNode *EQUALS();
    antlr4::tree::TerminalNode *NOTEQUALS();
    antlr4::tree::TerminalNode *NOTEQUALS_GT_LT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JoinOperatorContext* joinOperator();

  class  JoinTypeContext : public antlr4::ParserRuleContext {
  public:
    JoinTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INNER();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *FULL();
    antlr4::tree::TerminalNode *OUTER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  JoinTypeContext* joinType();

  class  FromTableContext : public antlr4::ParserRuleContext {
  public:
    FromTableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableContext *table();
    antlr4::tree::TerminalNode *AS();
    AliasContext *alias();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FromTableContext* fromTable();

  class  ColumnIdContext : public antlr4::ParserRuleContext {
  public:
    ColumnIdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnContext *column();
    TableContext *table();
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ColumnIdContext* columnId();

  class  TableContext : public antlr4::ParserRuleContext {
  public:
    TableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *DELIMID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TableContext* table();

  class  ColumnContext : public antlr4::ParserRuleContext {
  public:
    ColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *DELIMID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ColumnContext* column();

  class  DatabaseContext : public antlr4::ParserRuleContext {
  public:
    DatabaseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *DELIMID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DatabaseContext* database();

  class  AliasContext : public antlr4::ParserRuleContext {
  public:
    AliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *DELIMID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AliasContext* alias();

  class  IndexNameContext : public antlr4::ParserRuleContext {
  public:
    IndexNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *DELIMID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IndexNameContext* indexName();

  class  ConstraintNameContext : public antlr4::ParserRuleContext {
  public:
    ConstraintNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *DELIMID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstraintNameContext* constraintName();

  class  LimitContext : public antlr4::ParserRuleContext {
  public:
    LimitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTLIT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LimitContext* limit();

  class  OffsetContext : public antlr4::ParserRuleContext {
  public:
    OffsetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTLIT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  OffsetContext* offset();

  class  BlockSizeContext : public antlr4::ParserRuleContext {
  public:
    BlockSizeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTLIT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  BlockSizeContext* blockSize();

  class  ColumnValueContext : public antlr4::ParserRuleContext {
  public:
    ColumnValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTLIT();
    antlr4::tree::TerminalNode *FLOATLIT();
    GeometryContext *geometry();
    antlr4::tree::TerminalNode *NULL_T();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *DATETIMELIT();
    antlr4::tree::TerminalNode *BOOLEANLIT();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ColumnValueContext* columnValue();

  class  ConstraintContext : public antlr4::ParserRuleContext {
  public:
    ConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNIQUE();
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *NULL_T();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstraintContext* constraint();

  class  RetpayloadContext : public antlr4::ParserRuleContext {
  public:
    RetpayloadContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DatatypeContext *datatype();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  RetpayloadContext* retpayload();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExpressionContext() = default;
    void copyFrom(ExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DecimalLiteralContext : public ExpressionContext {
  public:
    DecimalLiteralContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *FLOATLIT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  CastOperationContext : public ExpressionContext {
  public:
    CastOperationContext(ExpressionContext *ctx);

    antlr4::Token *op = nullptr;
    antlr4::tree::TerminalNode *LPAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    DatatypeContext *datatype();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *CAST();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  GeoReferenceContext : public ExpressionContext {
  public:
    GeoReferenceContext(ExpressionContext *ctx);

    GeometryContext *geometry();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  DateTimeLiteralContext : public ExpressionContext {
  public:
    DateTimeLiteralContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *DATETIMELIT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  NowLiteralContext : public ExpressionContext {
  public:
    NowLiteralContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *NOW();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  IntLiteralContext : public ExpressionContext {
  public:
    IntLiteralContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *INTLIT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  TernaryOperationContext : public ExpressionContext {
  public:
    TernaryOperationContext(ExpressionContext *ctx);

    antlr4::Token *op = nullptr;
    antlr4::Token *op2 = nullptr;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *BETWEEN();
    antlr4::tree::TerminalNode *AND();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  AggregationContext : public ExpressionContext {
  public:
    AggregationContext(ExpressionContext *ctx);

    antlr4::Token *op = nullptr;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *MIN_AGG();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *MAX_AGG();
    antlr4::tree::TerminalNode *SUM_AGG();
    antlr4::tree::TerminalNode *COUNT_AGG();
    antlr4::tree::TerminalNode *ASTERISK();
    antlr4::tree::TerminalNode *AVG_AGG();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  BinaryOperationContext : public ExpressionContext {
  public:
    BinaryOperationContext(ExpressionContext *ctx);

    GpuSqlParser::ExpressionContext *left = nullptr;
    antlr4::Token *op = nullptr;
    GpuSqlParser::ExpressionContext *right = nullptr;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *ATAN2();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *POW();
    antlr4::tree::TerminalNode *ROOT();
    antlr4::tree::TerminalNode *ROUND();
    antlr4::tree::TerminalNode *POINT();
    antlr4::tree::TerminalNode *GEO_CONTAINS();
    antlr4::tree::TerminalNode *GEO_INTERSECT();
    antlr4::tree::TerminalNode *GEO_UNION();
    antlr4::tree::TerminalNode *GEO_LONGITUDE_TO_TILE_X();
    antlr4::tree::TerminalNode *GEO_LATITUDE_TO_TILE_Y();
    antlr4::tree::TerminalNode *GEO_TILE_X_TO_LONGITUDE();
    antlr4::tree::TerminalNode *GEO_TILE_Y_TO_LATITUDE();
    antlr4::tree::TerminalNode *CONCAT();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *DIVISION();
    antlr4::tree::TerminalNode *ASTERISK();
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *MODULO();
    antlr4::tree::TerminalNode *XOR();
    antlr4::tree::TerminalNode *BIT_AND();
    antlr4::tree::TerminalNode *BIT_OR();
    antlr4::tree::TerminalNode *L_SHIFT();
    antlr4::tree::TerminalNode *R_SHIFT();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATEREQ();
    antlr4::tree::TerminalNode *LESSEQ();
    antlr4::tree::TerminalNode *EQUALS();
    antlr4::tree::TerminalNode *NOTEQUALS();
    antlr4::tree::TerminalNode *NOTEQUALS_GT_LT();
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *OR();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  ParenExpressionContext : public ExpressionContext {
  public:
    ParenExpressionContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *LPAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RPAREN();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  UnaryOperationContext : public ExpressionContext {
  public:
    UnaryOperationContext(ExpressionContext *ctx);

    antlr4::Token *op = nullptr;
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *LOGICAL_NOT();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *ABS();
    antlr4::tree::TerminalNode *SIN();
    antlr4::tree::TerminalNode *COS();
    antlr4::tree::TerminalNode *TAN();
    antlr4::tree::TerminalNode *COT();
    antlr4::tree::TerminalNode *ASIN();
    antlr4::tree::TerminalNode *ACOS();
    antlr4::tree::TerminalNode *ATAN();
    antlr4::tree::TerminalNode *LOG10();
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *EXP();
    antlr4::tree::TerminalNode *SQRT();
    antlr4::tree::TerminalNode *SQUARE();
    antlr4::tree::TerminalNode *SIGN();
    antlr4::tree::TerminalNode *ROUND();
    antlr4::tree::TerminalNode *FLOOR();
    antlr4::tree::TerminalNode *CEIL();
    antlr4::tree::TerminalNode *DATETYPE();
    antlr4::tree::TerminalNode *YEAR();
    antlr4::tree::TerminalNode *MONTH();
    antlr4::tree::TerminalNode *DAY();
    antlr4::tree::TerminalNode *HOUR();
    antlr4::tree::TerminalNode *MINUTE();
    antlr4::tree::TerminalNode *SECOND();
    antlr4::tree::TerminalNode *WEEKDAY();
    antlr4::tree::TerminalNode *DAYOFWEEK();
    antlr4::tree::TerminalNode *LTRIM();
    antlr4::tree::TerminalNode *RTRIM();
    antlr4::tree::TerminalNode *LOWER();
    antlr4::tree::TerminalNode *UPPER();
    antlr4::tree::TerminalNode *REVERSE();
    antlr4::tree::TerminalNode *LEN();
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *NULL_T();
    antlr4::tree::TerminalNode *NOT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  VarReferenceContext : public ExpressionContext {
  public:
    VarReferenceContext(ExpressionContext *ctx);

    ColumnIdContext *columnId();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  PiLiteralContext : public ExpressionContext {
  public:
    PiLiteralContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *PI();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  StringLiteralContext : public ExpressionContext {
  public:
    StringLiteralContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *STRING();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  class  BooleanLiteralContext : public ExpressionContext {
  public:
    BooleanLiteralContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *BOOLEANLIT();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  DatatypeContext : public antlr4::ParserRuleContext {
  public:
    DatatypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTTYPE();
    antlr4::tree::TerminalNode *LONGTYPE();
    antlr4::tree::TerminalNode *DATETYPE();
    antlr4::tree::TerminalNode *DETETIMETYPE();
    antlr4::tree::TerminalNode *FLOATTYPE();
    antlr4::tree::TerminalNode *DOUBLETYPE();
    antlr4::tree::TerminalNode *STRINGTYPE();
    antlr4::tree::TerminalNode *BOOLEANTYPE();
    antlr4::tree::TerminalNode *POINTTYPE();
    antlr4::tree::TerminalNode *POLYTYPE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DatatypeContext* datatype();

  class  GeometryContext : public antlr4::ParserRuleContext {
  public:
    GeometryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PointGeometryContext *pointGeometry();
    PolygonGeometryContext *polygonGeometry();
    LineStringGeometryContext *lineStringGeometry();
    MultiPointGeometryContext *multiPointGeometry();
    MultiLineStringGeometryContext *multiLineStringGeometry();
    MultiPolygonGeometryContext *multiPolygonGeometry();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  GeometryContext* geometry();

  class  PointGeometryContext : public antlr4::ParserRuleContext {
  public:
    PointGeometryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *POINT();
    antlr4::tree::TerminalNode *LPAREN();
    PointContext *point();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PointGeometryContext* pointGeometry();

  class  LineStringGeometryContext : public antlr4::ParserRuleContext {
  public:
    LineStringGeometryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LINESTRING();
    LineStringContext *lineString();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LineStringGeometryContext* lineStringGeometry();

  class  PolygonGeometryContext : public antlr4::ParserRuleContext {
  public:
    PolygonGeometryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *POLYGON();
    PolygonContext *polygon();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PolygonGeometryContext* polygonGeometry();

  class  MultiPointGeometryContext : public antlr4::ParserRuleContext {
  public:
    MultiPointGeometryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MULTIPOINT();
    antlr4::tree::TerminalNode *LPAREN();
    std::vector<PointOrClosedPointContext *> pointOrClosedPoint();
    PointOrClosedPointContext* pointOrClosedPoint(size_t i);
    antlr4::tree::TerminalNode *RPAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MultiPointGeometryContext* multiPointGeometry();

  class  MultiLineStringGeometryContext : public antlr4::ParserRuleContext {
  public:
    MultiLineStringGeometryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MULTILINESTRING();
    antlr4::tree::TerminalNode *LPAREN();
    std::vector<LineStringContext *> lineString();
    LineStringContext* lineString(size_t i);
    antlr4::tree::TerminalNode *RPAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MultiLineStringGeometryContext* multiLineStringGeometry();

  class  MultiPolygonGeometryContext : public antlr4::ParserRuleContext {
  public:
    MultiPolygonGeometryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MULTIPOLYGON();
    antlr4::tree::TerminalNode *LPAREN();
    std::vector<PolygonContext *> polygon();
    PolygonContext* polygon(size_t i);
    antlr4::tree::TerminalNode *RPAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MultiPolygonGeometryContext* multiPolygonGeometry();

  class  PointOrClosedPointContext : public antlr4::ParserRuleContext {
  public:
    PointOrClosedPointContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PointContext *point();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PointOrClosedPointContext* pointOrClosedPoint();

  class  PolygonContext : public antlr4::ParserRuleContext {
  public:
    PolygonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    std::vector<LineStringContext *> lineString();
    LineStringContext* lineString(size_t i);
    antlr4::tree::TerminalNode *RPAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PolygonContext* polygon();

  class  LineStringContext : public antlr4::ParserRuleContext {
  public:
    LineStringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    std::vector<PointContext *> point();
    PointContext* point(size_t i);
    antlr4::tree::TerminalNode *RPAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LineStringContext* lineString();

  class  PointContext : public antlr4::ParserRuleContext {
  public:
    PointContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> FLOATLIT();
    antlr4::tree::TerminalNode* FLOATLIT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> INTLIT();
    antlr4::tree::TerminalNode* INTLIT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  PointContext* point();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

