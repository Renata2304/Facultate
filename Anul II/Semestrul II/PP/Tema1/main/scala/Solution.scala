import util.Pixel
import util.Util

// Online viewer: https://0xc0de.fr/webppm/
object Solution {
  type Image = List[List[Pixel]]
  type GrayscaleImage = List[List[Double]]

  // prerequisites
  def fromStringPPM(image: List[Char]): Image = {
    def splitWithDelims(s: List[Char], delim: List[Char]): List[List[Char]] = {
      def op(crtChar: Char, acc: List[List[Char]]): List[List[Char]] =
        // daca caracterul primit este de tip delimitator, se sare peste
        if (delim.contains(crtChar)) Nil :: acc
        else acc match {
          case Nil => List(List(crtChar)) // se adauga primul caracter
          case x :: xs => (crtChar :: x) :: xs // se adauga restul caracterelor
        }
      s.foldRight(Nil: List[List[Char]])((c, acc) => op(c, acc))
    }

    // renuntarea la caracterele 'P' si '3'
    val img = splitWithDelims(image.drop(3), " \n".toList)
    val pixels = img.map(_.mkString.toInt).drop(3)
    val n = img.map(_.mkString.toInt).head // nr de coloane pe care trebuie sa il aiba matrice

    /* se parcurg listele de int-uri si se creeaza pixelii, acestia fiind pe urma grupati
       si dupa nr de coloane, pentru a se forma matricea finala
    */
    pixels.grouped(3).toList.map(
      { case List(r, g, b) => Pixel(r, g ,b)}
    ).grouped(n).toList
  }

  def toStringPPM(image: Image): List[Char] = {
    /* crearea primelor 3 randuri ale output-ului, constand in:
       'P' '3'
        nr de coloane (image.head.length), nr de linii (image.length)
       '2''5''5'
    */
    val info: List[Char] = List('P', '3', '\n') ++
      image.head.length.toString.toList ++ List(' ') ++ image.length.toString.toList ++
      List('\n', '2', '5', '5', '\n')

    // fiecare fixel este impartit in r,g,b si adaugat pe rand in lista
    def pixelToString(crtPixel: Pixel): List[Char] = {
      crtPixel.red.toString.toList ++
        " ".toList ++
        crtPixel.green.toString.toList ++
        " ".toList ++
        crtPixel.blue.toString.toList ++ List('\n')
    }

    val pixelChars: List[Char] = image.flatten.flatMap(pixelToString)

    info ::: pixelChars // adaugarea informatiilor la inceputul listei de pixeli
  }

  // ex 1
  def verticalConcat(image1: Image, image2: Image): Image = {
    image1 ::: image2  // se adauga imaginea 1 in fata imaginii 2
  }

  // ex 2
  def horizontalConcat(image1: Image, image2: Image): Image = {
    // concatenarea celor doua matrici de pixeli, linie cu linie
    image1.zip(image2).map { case (row1, row2) => row1 ++ row2 }
  }

  // ex 3
  def rotate(image: Image, degrees: Integer): Image = {
    degrees match {
      case 90 =>
        image.transpose.reverse // rotatie de 90 de grade
      case 180 =>
        image.reverse.map(_.reverse) // rotatie de 180 de grade
      case 270 =>
        image.transpose.map(_.reverse) // rotatie de 270 de grade
      case _ =>
        image // imaginea se intoarce in pozitia initiala
    }
  }

  // ex 4
  val gaussianBlurKernel: GrayscaleImage = List[List[Double]](
    List( 1, 4, 7, 4, 1),
    List( 4,16,26,16, 4),
    List( 7,26,41,26, 7),
    List( 4,16,26,16, 4),
    List( 1, 4, 7, 4, 1)
  ).map(_.map(_ / 273))

  val Gx : GrayscaleImage = List(
    List(-1, 0, 1),
    List(-2, 0, 2),
    List(-1, 0, 1)
  )

  val Gy : GrayscaleImage = List(
    List( 1, 2, 1),
    List( 0, 0, 0),
    List(-1,-2,-1)
  )

  def edgeDetection(image: Image, threshold : Double): Image = {
    // trecerea imaginii in grayscale
    val grayImage = image.map(_.map(pixel => Util.toGrayScale(pixel)))
    val matrix = applyConvolution(grayImage, gaussianBlurKernel)
    val matrix1 = applyConvolution(matrix, Gx)
    val matrix2 = applyConvolution(matrix, Gy)

    val finalMatrix = matrix1.zip(matrix2).map { case (row1, row2) =>
      row1.zip(row2).map { case (elem1, elem2) =>
        elem1.abs + elem2.abs
      }
    }

    finalMatrix.map { row =>
      row.map { x =>
        if (x < threshold) Pixel(0, 0, 0)
        else Pixel(255, 255, 255)
      }
    }
  }

  def applyConvolution(image: GrayscaleImage, kernel: GrayscaleImage) : GrayscaleImage = {
    /*  folosind functia din Util, se vor construi vecinii de raza (kernel.size / 2)
        si apoi se vor inmulti kernelul si matricile generate
     */
    Util.getNeighbors(image, kernel.size / 2).map(_.map(image =>
        image.flatten.zip(kernel.flatten).map(p => p._1 * p._2).sum))
  }

  // ex 5
  def moduloPascal(m: Integer, funct: Integer => Pixel, size: Integer): Image = {
    def generateNextRow(currentRow: List[Int]): List[Int] = {
      // se adauga la vechea lista cate un '0' in ambele capete
      val newRow = 0 :: currentRow ::: List(0)
      // se calculeaza suma a doua cate doua elemente, retinandu-se restul impartirii sumei la m
      newRow.sliding(2).map(_.sum % m).toList
    }

    def generatePascalTriangle(numRows: Int): List[List[Int]] = {
      // daca s-a introdus o valoare invalida
      if (numRows == 0) {
        List.empty
      } else {
        /* prima linie din triunghiul lui Lascal va fi List(1)
           se va folosi iterate pentru a construi triunghiul lui Pascal, linie cu linie
        */
        List.iterate(List(1), numRows)(generateNextRow)
      }
    }

    // generarea triunghiului lui Pascal
    val pascal = generatePascalTriangle(size)

    // pe fiecare linie se vor pune -1 pentru a se crea matricea de size x size
    val matrix = pascal.map(row => row ++ List.fill(size - row.size)(-1))

    // folosirea functiei din Tests pentru a transforma in-urile in pixeli
    matrix.map(_.map(elem =>
      if (elem == -1) Pixel(0,0,0)
      else funct(elem)))
  }
}