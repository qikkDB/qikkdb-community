
// Generated from C:/Users/Andy/Desktop/parser\GpuSqlParser.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  GpuSqlParser : public antlr4::Parser {
public:
  enum {
    DATETIMELIT = 1, LF = 2, CR = 3, CRLF = 4, WS = 5, SEMICOL = 6, SQOUTE = 7, 
    DQOUTE = 8, UNDERSCORE = 9, COLON = 10, COMMA = 11, DOT = 12, STRING = 13, 
    DELIMID = 14, DATELIT = 15, TIMELIT = 16, POINT = 17, MULTIPOINT = 18, 
    LINESTRING = 19, MULTILINESTRING = 20, POLYGON = 21, MULTIPOLYGON = 22, 
    DATATYPE = 23, INTTYPE = 24, LONGTYPE = 25, DATETYPE = 26, FLOATTYPE = 27, 
    DOUBLETYPE = 28, STRINGTYPE = 29, BOOLEANTYPE = 30, POINTTYPE = 31, 
    POLYTYPE = 32, INSERTINTO = 33, CREATEDB = 34, DROPDB = 35, CREATETABLE = 36, 
    DROPTABLE = 37, ALTERTABLE = 38, ADD = 39, DROPCOLUMN = 40, ALTERCOLUMN = 41, 
    CREATEINDEX = 42, INDEX = 43, PRIMARYKEY = 44, VALUES = 45, SELECT = 46, 
    FROM = 47, JOIN = 48, WHERE = 49, GROUPBY = 50, AS = 51, IN = 52, ISNULL = 53, 
    ISNOTNULL = 54, BETWEEN = 55, ON = 56, ORDERBY = 57, DIR = 58, LIMIT = 59, 
    OFFSET = 60, INNER = 61, FULLOUTER = 62, SHOWDB = 63, SHOWTB = 64, SHOWCL = 65, 
    AGG = 66, AVG = 67, SUM = 68, MIN = 69, MAX = 70, COUNT = 71, YEAR = 72, 
    MONTH = 73, DAY = 74, HOUR = 75, MINUTE = 76, SECOND = 77, NOW = 78, 
    PI = 79, ABS = 80, SIN = 81, COS = 82, TAN = 83, COT = 84, ASIN = 85, 
    ACOS = 86, ATAN = 87, ATAN2 = 88, LOG10 = 89, LOG = 90, EXP = 91, POW = 92, 
    SQRT = 93, SQUARE = 94, SIGN = 95, ROOT = 96, ROUND = 97, CEIL = 98, 
    FLOOR = 99, LTRIM = 100, RTRIM = 101, LOWER = 102, UPPER = 103, REVERSE = 104, 
    LEN = 105, LEFT = 106, RIGHT = 107, CONCAT = 108, CAST = 109, GEO_CONTAINS = 110, 
    GEO_INTERSECT = 111, GEO_UNION = 112, PLUS = 113, MINUS = 114, ASTERISK = 115, 
    DIVISION = 116, MODULO = 117, XOR = 118, EQUALS = 119, NOTEQUALS = 120, 
    NOTEQUALS_GT_LT = 121, LPAREN = 122, RPAREN = 123, GREATER = 124, LESS = 125, 
    GREATEREQ = 126, LESSEQ = 127, LOGICAL_NOT = 128, OR = 129, AND = 130, 
    BIT_OR = 131, BIT_AND = 132, L_SHIFT = 133, R_SHIFT = 134, BOOLEANLIT = 135, 
    TRUE = 136, FALSE = 137, FLOATLIT = 138, INTLIT = 139, NULLLIT = 140, 
    ID = 141
  };

  enum {
    RuleSqlFile = 0, RuleStatement = 1, RuleShowStatement = 2, RuleShowDatabases = 3, 
    RuleShowTables = 4, RuleShowColumns = 5, RuleSqlSelect = 6, RuleSqlCreateDb = 7, 
    RuleSqlDropDb = 8, RuleSqlCreateTable = 9, RuleSqlDropTable = 10, RuleSqlAlterTable = 11, 
    RuleSqlCreateIndex = 12, RuleSqlInsertInto = 13, RuleNewTableEntries = 14, 
    RuleNewTableEntry = 15, RuleAlterTableEntries = 16, RuleAlterTableEntry = 17, 
    RuleAddColumn = 18, RuleDropColumn = 19, RuleAlterColumn = 20, RuleNewTableColumn = 21, 
    RuleNewTableIndex = 22, RuleSelectColumns = 23, RuleSelectColumn = 24, 
    RuleSelectAllColumns = 25, RuleWhereClause = 26, RuleOrderByColumns = 27, 
    RuleOrderByColumn = 28, RuleInsertIntoValues = 29, RuleInsertIntoColumns = 30, 
    RuleIndexColumns = 31, RuleGroupByColumns = 32, RuleGroupByColumn = 33, 
    RuleFromTables = 34, RuleJoinClauses = 35, RuleJoinClause = 36, RuleJoinTable = 37, 
    RuleJoinColumnLeft = 38, RuleJoinColumnRight = 39, RuleJoinOperator = 40, 
    RuleJoinType = 41, RuleFromTable = 42, RuleColumnId = 43, RuleTable = 44, 
    RuleColumn = 45, RuleDatabase = 46, RuleAlias = 47, RuleIndexName = 48, 
    RuleLimit = 49, RuleOffset = 50, RuleBlockSize = 51, RuleColumnValue = 52, 
    RuleExpression = 53, RuleGeometry = 54, RulePointGeometry = 55, RuleLineStringGeometry = 56, 
    RulePolygonGeometry = 57, RuleMultiPointGeometry = 58, RuleMultiLineStringGeometry = 59, 
    RuleMultiPolygonGeometry = 60, RulePointOrClosedPoint = 61, RulePolygon = 62, 
    RuleLineString = 63, RulePoint = 64
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
  class SqlSelectContext;
  class SqlCreateDbContext;
  class SqlDropDbContext;
  class SqlCreateTableContext;
  class SqlDropTableContext;
  class SqlAlterTableContext;
  class SqlCreateIndexContext;
  class SqlInsertIntoContext;
  class NewTableEntriesContext;
  class NewTableEntryContext;
  class AlterTableEntriesContext;
  class AlterTableEntryContext;
  class AddColumnContext;
  class DropColumnContext;
  class AlterColumnContext;
  class NewTableColumnContext;
  class NewTableIndexContext;
  class SelectColumnsContext;
  class SelectColumnContext;
  class SelectAllColumnsContext;
  class WhereClauseContext;
  class OrderByColumnsContext;
  class OrderByColumnContext;
  class InsertIntoValuesContext;
  class InsertIntoColumnsContext;
  class IndexColumnsContext;
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
  class LimitContext;
  class OffsetContext;
  class BlockSizeContext;
  class ColumnValueContext;
  class ExpressionContext;
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
    SqlCreateIndexContext *sqlCreateIndex();
    SqlInsertIntoContext *sqlInsertInto();
    ShowStatementContext *showStatement();

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

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowStatementContext* showStatement();

  class  ShowDatabasesContext : public antlr4::ParserRuleContext {
  public:
    ShowDatabasesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOWDB();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ShowDatabasesContext* showDatabases();

  class  ShowTablesContext : public antlr4::ParserRuleContext {
  public:
    ShowTablesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOWTB();
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
    antlr4::tree::TerminalNode *SHOWCL();
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
    antlr4::tree::TerminalNode *GROUPBY();
    GroupByColumnsContext *groupByColumns();
    antlr4::tree::TerminalNode *ORDERBY();
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
    antlr4::tree::TerminalNode *CREATEDB();
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
    antlr4::tree::TerminalNode *DROPDB();
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
    antlr4::tree::TerminalNode *CREATETABLE();
    TableContext *table();
    antlr4::tree::TerminalNode *LPAREN();
    NewTableEntriesContext *newTableEntries();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlCreateTableContext* sqlCreateTable();

  class  SqlDropTableContext : public antlr4::ParserRuleContext {
  public:
    SqlDropTableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROPTABLE();
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
    antlr4::tree::TerminalNode *ALTERTABLE();
    TableContext *table();
    AlterTableEntriesContext *alterTableEntries();
    antlr4::tree::TerminalNode *SEMICOL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  SqlAlterTableContext* sqlAlterTable();

  class  SqlCreateIndexContext : public antlr4::ParserRuleContext {
  public:
    SqlCreateIndexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATEINDEX();
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
    antlr4::tree::TerminalNode *INSERTINTO();
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
    NewTableIndexContext *newTableIndex();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NewTableEntryContext* newTableEntry();

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
    antlr4::tree::TerminalNode *DATATYPE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AddColumnContext* addColumn();

  class  DropColumnContext : public antlr4::ParserRuleContext {
  public:
    DropColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROPCOLUMN();
    ColumnContext *column();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  DropColumnContext* dropColumn();

  class  AlterColumnContext : public antlr4::ParserRuleContext {
  public:
    AlterColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTERCOLUMN();
    ColumnContext *column();
    antlr4::tree::TerminalNode *DATATYPE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterColumnContext* alterColumn();

  class  NewTableColumnContext : public antlr4::ParserRuleContext {
  public:
    NewTableColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnContext *column();
    antlr4::tree::TerminalNode *DATATYPE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NewTableColumnContext* newTableColumn();

  class  NewTableIndexContext : public antlr4::ParserRuleContext {
  public:
    NewTableIndexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INDEX();
    IndexNameContext *indexName();
    antlr4::tree::TerminalNode *LPAREN();
    IndexColumnsContext *indexColumns();
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  NewTableIndexContext* newTableIndex();

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
    antlr4::tree::TerminalNode *FULLOUTER();

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
    antlr4::tree::TerminalNode *NULLLIT();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ColumnValueContext* columnValue();

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
    antlr4::tree::TerminalNode *DATATYPE();
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

    antlr4::tree::TerminalNode *AGG();
    antlr4::tree::TerminalNode *LPAREN();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RPAREN();
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
    antlr4::tree::TerminalNode *POINT();
    antlr4::tree::TerminalNode *GEO_CONTAINS();
    antlr4::tree::TerminalNode *GEO_INTERSECT();
    antlr4::tree::TerminalNode *GEO_UNION();
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
    antlr4::tree::TerminalNode *YEAR();
    antlr4::tree::TerminalNode *MONTH();
    antlr4::tree::TerminalNode *DAY();
    antlr4::tree::TerminalNode *HOUR();
    antlr4::tree::TerminalNode *MINUTE();
    antlr4::tree::TerminalNode *SECOND();
    antlr4::tree::TerminalNode *LTRIM();
    antlr4::tree::TerminalNode *RTRIM();
    antlr4::tree::TerminalNode *LOWER();
    antlr4::tree::TerminalNode *UPPER();
    antlr4::tree::TerminalNode *REVERSE();
    antlr4::tree::TerminalNode *LEN();
    antlr4::tree::TerminalNode *ISNULL();
    antlr4::tree::TerminalNode *ISNOTNULL();
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

