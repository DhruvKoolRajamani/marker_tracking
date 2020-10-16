#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

String rootPath = "resources/markers";

int main(int, char**)
{
  int markersX, markersY, markerLength, markerSeparation, dictionaryId, margins,
      borderBits;

  markersX = 4;
  markersY = 4;
  markerLength = 20;
  markerSeparation = 2;
  dictionaryId = aruco::DICT_4X4_50;
  margins = 4;
  borderBits = 1;
  String out = "marker_board.png";

  Size imageSize;
  imageSize.width = markersX * (markerLength + markerSeparation) -
                    markerSeparation + 2 * margins;
  imageSize.height = markersY * (markerLength + markerSeparation) -
                     markerSeparation + 2 * margins;

  Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(
      aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));

  Ptr<aruco::GridBoard> board =
      aruco::GridBoard::create(markersX, markersY, float(markerLength),
                               float(markerSeparation), dictionary);

  // show created board
  Mat boardImage;
  board->draw(imageSize, boardImage, margins, borderBits);

  if (true)
  {
    imshow("marker_board", boardImage);
    waitKey(0);
  }

  imwrite(out, boardImage);

  return 0;
}