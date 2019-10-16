
// Generated from C:/GPU-DB/dropdbase/GpuSqlParser\GpuSqlParser.g4 by ANTLR 4.7.2

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
    BOOLEANTYPE = 32, POINTTYPE = 33, POLYTYPE = 34, INSERTINTO = 35, CREATEDB = 36, 
    DROPDB = 37, CREATETABLE = 38, DROPTABLE = 39, ALTERTABLE = 40, ALTERDATABASE = 41, 
    DROPCOLUMN = 42, ALTERCOLUMN = 43, RENAMECOLUMN = 44, RENAMETO = 45, 
    CREATEINDEX = 46, INDEX = 47, UNIQUE = 48, PRIMARYKEY = 49, CREATE = 50, 
    ADD = 51, DROP = 52, ALTER = 53, RENAME = 54, DATABASE = 55, TABLE = 56, 
    COLUMN = 57, VALUES = 58, SELECT = 59, FROM = 60, JOIN = 61, WHERE = 62, 
    GROUPBY = 63, AS = 64, IN = 65, TO = 66, ISNULL = 67, ISNOTNULL = 68, 
    NOTNULL = 69, BETWEEN = 70, ON = 71, ORDERBY = 72, DIR = 73, LIMIT = 74, 
    OFFSET = 75, INNER = 76, FULLOUTER = 77, SHOWDB = 78, SHOWTB = 79, SHOWCL = 80, 
    SHOWQTYPES = 81, AVG_AGG = 82, SUM_AGG = 83, MIN_AGG = 84, MAX_AGG = 85, 
    COUNT_AGG = 86, YEAR = 87, MONTH = 88, DAY = 89, HOUR = 90, MINUTE = 91, 
    SECOND = 92, NOW = 93, PI = 94, ABS = 95, SIN = 96, COS = 97, TAN = 98, 
    COT = 99, ASIN = 100, ACOS = 101, ATAN = 102, ATAN2 = 103, LOG10 = 104, 
    LOG = 105, EXP = 106, POW = 107, SQRT = 108, SQUARE = 109, SIGN = 110, 
    ROOT = 111, ROUND = 112, CEIL = 113, FLOOR = 114, LTRIM = 115, RTRIM = 116, 
    LOWER = 117, UPPER = 118, REVERSE = 119, LEN = 120, LEFT = 121, RIGHT = 122, 
    CONCAT = 123, CAST = 124, GEO_CONTAINS = 125, GEO_INTERSECT = 126, GEO_UNION = 127, 
    PLUS = 128, MINUS = 129, ASTERISK = 130, DIVISION = 131, MODULO = 132, 
    XOR = 133, EQUALS = 134, NOTEQUALS = 135, NOTEQUALS_GT_LT = 136, LPAREN = 137, 
    RPAREN = 138, GREATER = 139, LESS = 140, GREATEREQ = 141, LESSEQ = 142, 
    LOGICAL_NOT = 143, OR = 144, AND = 145, BIT_OR = 146, BIT_AND = 147, 
    L_SHIFT = 148, R_SHIFT = 149, BOOLEANLIT = 150, TRUE = 151, FALSE = 152, 
    FLOATLIT = 153, INTLIT = 154, NULLLIT = 155, ID = 156
  };

  enum {
    RuleSqlFile = 0, RuleStatement = 1, RuleShowStatement = 2, RuleShowDatabases = 3, 
    RuleShowTables = 4, RuleShowColumns = 5, RuleShowQueryTypes = 6, RuleSqlSelect = 7, 
    RuleSqlCreateDb = 8, RuleSqlDropDb = 9, RuleSqlCreateTable = 10, RuleSqlDropTable = 11, 
    RuleSqlAlterTable = 12, RuleSqlAlterDatabase = 13, RuleSqlCreateIndex = 14, 
    RuleSqlInsertInto = 15, RuleNewTableEntries = 16, RuleNewTableEntry = 17, 
    RuleAlterDatabaseEntries = 18, RuleAlterDatabaseEntry = 19, RuleRenameDatabase = 20, 
    RuleAlterTableEntries = 21, RuleAlterTableEntry = 22, RuleAddColumn = 23, 
    RuleDropColumn = 24, RuleAlterColumn = 25, RuleRenameColumn = 26, RuleRenameTable = 27, 
    RuleAddConstraint = 28, RuleDropConstraint = 29, RuleRenameColumnFrom = 30, 
    RuleRenameColumnTo = 31, RuleNewTableColumn = 32, RuleNewTableConstraint = 33, 
    RuleSelectColumns = 34, RuleSelectColumn = 35, RuleSelectAllColumns = 36, 
    RuleWhereClause = 37, RuleOrderByColumns = 38, RuleOrderByColumn = 39, 
    RuleInsertIntoRows = 40, RuleInsertIntoValues = 41, RuleInsertIntoColumns = 42, 
    RuleIndexColumns = 43, RuleConstraintColumns = 44, RuleGroupByColumns = 45, 
    RuleGroupByColumn = 46, RuleFromTables = 47, RuleJoinClauses = 48, RuleJoinClause = 49, 
    RuleJoinTable = 50, RuleJoinColumnLeft = 51, RuleJoinColumnRight = 52, 
    RuleJoinOperator = 53, RuleJoinType = 54, RuleFromTable = 55, RuleColumnId = 56, 
    RuleTable = 57, RuleColumn = 58, RuleDatabase = 59, RuleAlias = 60, 
    RuleIndexName = 61, RuleConstraintName = 62, RuleLimit = 63, RuleOffset = 64, 
    RuleBlockSize = 65, RuleColumnValue = 66, RuleConstraint = 67, RuleExpression = 68, 
    RuleDatatype = 69, RuleGeometry = 70, RulePointGeometry = 71, RuleLineStringGeometry = 72, 
    RulePolygonGeometry = 73, RuleMultiPointGeometry = 74, RuleMultiLineStringGeometry = 75, 
    RuleMultiPolygonGeometry = 76, RulePointOrClosedPoint = 77, RulePolygon = 78, 
    RuleLineString = 79, RulePoint = 80
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
  class InsertIntoRowsContext;
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

  class  ShowQueryTypesContext : public antlr4::ParserRuleContext {
  public:
    ShowQueryTypesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SHOWQTYPES();
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
    BlockSizeContext *blockSize();

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

  class  SqlAlterDatabaseContext : public antlr4::ParserRuleContext {
  public:
    SqlAlterDatabaseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALTERDATABASE();
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
    antlr4::tree::TerminalNode *LPAREN();
    InsertIntoColumnsContext *insertIntoColumns();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *VALUES();
    InsertIntoRowsContext *insertIntoRows();
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

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterDatabaseEntryContext* alterDatabaseEntry();

  class  RenameDatabaseContext : public antlr4::ParserRuleContext {
  public:
    RenameDatabaseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RENAMETO();
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
    DatatypeContext *datatype();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AlterColumnContext* alterColumn();

  class  RenameColumnContext : public antlr4::ParserRuleContext {
  public:
    RenameColumnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RENAMECOLUMN();
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
    antlr4::tree::TerminalNode *RENAMETO();
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

  class  InsertIntoRowsContext : public antlr4::ParserRuleContext {
  public:
    InsertIntoRowsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InsertIntoValuesContext *> insertIntoValues();
    InsertIntoValuesContext* insertIntoValues(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  InsertIntoRowsContext* insertIntoRows();

  class  InsertIntoValuesContext : public antlr4::ParserRuleContext {
  public:
    InsertIntoValuesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
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
    antlr4::tree::TerminalNode *NULLLIT();
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
    antlr4::tree::TerminalNode *NOTNULL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ConstraintContext* constraint();

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

