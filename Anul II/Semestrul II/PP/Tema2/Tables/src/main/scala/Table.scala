import util.Util.{Line, Row}

trait FilterCond {
  def &&(other: FilterCond): FilterCond = new And(this, other)
  def ||(other: FilterCond): FilterCond = new Or(this, other)
  // fails if the column name is not present in the row
  def eval(r: Row): Option[Boolean]
}
case class Field(colName: String, predicate: String => Boolean) extends FilterCond {
  // 2.2.
  override def eval(r: Row): Option[Boolean] = {
    r.get(colName).flatMap { value =>
      if (predicate(value)) Some(true)
      else Some(false)
    }
  }
}

case class And(f1: FilterCond, f2: FilterCond) extends FilterCond {
  // 2.2.
  override def eval(r: Row): Option[Boolean] = {
    val result1 = f1.eval(r)
    val result2 = f2.eval(r)
    for {
      res1 <- result1
      res2 <- result2
    } yield res1 && res2
  }

}

case class Or(f1: FilterCond, f2: FilterCond) extends FilterCond {
  // 2.2.
  override def eval(r: Row): Option[Boolean] = {
    val result1 = f1.eval(r)
    val result2 = f2.eval(r)
    for {
      res1 <- result1
      res2 <- result2
    } yield res1 || res2
  }
}

trait Query {
  def eval: Option[Table]
}

/*
  Atom query which evaluates to the input table
  Always succeeds
 */
case class Value(t: Table) extends Query {
  override def eval: Option[Table] = Some(t)
}
/*
  Selects certain columns from the result of a target query
  Fails with None if some rows are not present in the resulting table
 */
case class Select(columns: Line, target: Query) extends Query {
  override def eval: Option[Table] = {
    target.eval.map { table =>
      table.select(columns).get
    }
  }
}

/*
  Filters rows from the result of the target query
  Success depends only on the success of the target
 */
case class Filter(condition: FilterCond, target: Query) extends Query {
  override def eval: Option[Table] = {
    target.eval.flatMap { table =>
      table.filter(condition)
    }
  }
}

/*
  Creates a new column with default values
  Success depends only on the success of the target
 */
case class NewCol(name: String, defaultVal: String, target: Query) extends Query {
  override def eval: Option[Table] = {
    target.eval match {
      case Some(table) => {
        val newTable = table.newCol(name, defaultVal)
        Some(newTable)
      }
      case other => other
    }
  }
}

/*
  Combines two tables based on a common key
  Success depends on whether the key exists in both tables or not AND on the success of the target
 */
case class Merge(key: String, t1: Query, t2: Query) extends Query {
  override def eval: Option[Table] = {
    for {
      table1 <- t1.eval
      table2 <- t2.eval
      mergedTable <- table1.merge(key, table2)
    } yield mergedTable
  }
}

class Table (columnNames: Line, tabular: List[List[String]]) {
  def getColumnNames: Line = columnNames

  def getTabular: List[List[String]] = tabular

  // 1.1
  override def toString: String = {
    // se creaza un string cu separator ","
    val rows = tabular.map(_.mkString(",")).mkString("\n")
    // coloanele sunt si ele impartite in cuvinte cu "," ca separator
    columnNames.mkString(",") + '\n'.toString + rows
  }

  // 2.1
  def select(columns: Line): Option[Table] = {
    val selectedColumnNames = columnNames.filter(columns.contains)
    if (selectedColumnNames.isEmpty) return None

    // indexez coloanele selectate
    val selectedColumnIndices = columnNames.zipWithIndex.filter { case (col, idx) =>
      columns.contains(col)
    }

    // selectez randurile corespunzatoare coloanelor selectate
    val selectedTabularData = tabular.map { row =>
      selectedColumnIndices.map { case (col, idx) => row(idx) }
    }

    Some(new Table(columns, selectedTabularData))
  }

  // 2.2
  def filter(cond: FilterCond): Option[Table] = {
    val filteredRows = tabular.filter(row => cond.eval(columnNames.zip(row).toMap).getOrElse(false))
    if (filteredRows.isEmpty) {
      None
    } else {
      Some(new Table(columnNames, filteredRows))
    }
  }

  // 2.3.
  def newCol(name: String, defaultVal: String): Table = {
    val newColumn = columnNames :+ name
    val newRows = tabular.map(row => row :+ defaultVal)
    new Table(newColumn, newRows)
  }

  // 2.4.
  def merge(key: String, other: Table): Option[Table] = {
    val keyFound = this.columnNames.contains(key) && other.getColumnNames.contains(key)
    // vericare daca exista key in ambele tabele
    if (!keyFound) {
      return None
    }

    // transformarea randurile din tabele in map uri
    val mappedTable1 = this.tabular.map { row =>
      columnNames.zip(row).toMap
    }
    val mappedTable2 = other.getTabular.map { rows =>
      other.getColumnNames.zip(rows).toMap
    }

    // salvarea coloanelor din al doilea tabel, care nu contin key
    val otherColumns: List[String] = other.getColumnNames.filter(_ != key)
    // salvarea coloanelor distincte din cele doua tabele
    val resultColumns: List[String] = columnNames ++ otherColumns.filterNot(columnNames.contains(_))
    // obtinerea tuturor valorilor distincte pentru key din ambele tabele
    val keys = (mappedTable1 ++ mappedTable2).flatMap(_.get(key)).distinct

    val mergedTabular: List[List[String]] = keys.sorted.map { k =>
      val row1 = mappedTable1.find { // randurile din prima tabela cu cheia k
        case row if row.getOrElse(key, "") == k => true
        case _ => false
      }.getOrElse(Map.empty)

      val row2 = mappedTable2.find { // randurile din prima tabela cu cheia k
        case row if row.get(key).contains(k) => true
        case _ => false
      }.getOrElse(Map.empty)

      // pentru fiecare coloana, combin valorile celor doua randuri
      resultColumns.map { col =>
        val valuePair = (row1.get(col), row2.get(col))
        valuePair match {
          // daca cele doua valori sunt diferite, ele se concateneaza
          case (Some(val1), Some(val2)) if val1 != val2 => val1 ++ ";" ++ val2
          // daca cele doua valori sunt identice, se pastreaza doar una dintre ele
          case (Some(val1), Some(val2)) if val1 == val2 => val1
          case (Some(val1), _) => val1
          case (_, Some(val2)) => val2
          case _ => ""
        }
      }
    }

    Some(new Table(resultColumns, mergedTabular))
  }

}


object Table {
  // 1.2
  def apply(s: String): Table = {
    // impartim randurile dupa "\n"
    val lines = s.split("\n").toList
    val columnNames = lines.head.split(",").toList
    // -1 pentru a nu se trece peste "" de la final de rand
    val tabularData = lines.tail.map(_.split(",", -1).toList)
    new Table(columnNames, tabularData)
  }
}
