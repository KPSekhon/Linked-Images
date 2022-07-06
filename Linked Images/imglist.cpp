// File:        imglist.cpp
// Date:        2022-01-27 10:21
// Description: Contains partial implementation of ImgList class
//              for CPSC 221 2021W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "imglist.h"

#include <math.h> // provides fmin and fabs functions

/**************************
* MISCELLANEOUS FUNCTIONS *
**************************/


/*
 * This function is NOT part of the ImgList class,
 * but will be useful for one of the ImgList functions.
 * Returns the "difference" between two hue values.
 * PRE: hue1 is a double between [0,360).
 * PRE: hue2 is a double between [0,360).
 *
 * The hue difference is the absolute difference between two hues,
 * but takes into account differences spanning the 360 value.
 * e.g. Two pixels with hues 90 and 110 differ by 20, but
 *      two pixels with hues 5 and 355 differ by 10.
 */
double HueDiff(double hue1, double hue2)
{
  return fmin(fabs(hue1 - hue2), fabs(hue2 + 360 - hue1));
}

/*********************
 * CONSTRUCTORS, ETC. *
 *********************/

/*
 * Default constructor. Makes an empty list
 */
ImgList::ImgList()
{
  // set appropriate values for all member attributes here
  this->northwest = nullptr;
  this->southeast = nullptr;
  return;
}

/*
 * Creates a list from image data
 * PRE: img has dimensions of at least 1x1
 */
ImgList::ImgList(PNG &img)
{
  // build the linked node structure and set the member attributes appropriately
  if (img.width() <= 0 || img.height() <= 0)
  {
    this->northwest = nullptr;
    this->southeast = nullptr;
    return;
  }
  ImgNode *currNode, *head, *currRow, *prevRow;
  northwest = nullptr;
  southeast = nullptr;
  currNode = makeRow(0, img);
  prevRow = currNode;

  for (unsigned int y = 1; y < img.height(); y++)
  {
    currNode = makeRow(y, img);
    currRow = currNode;
    head = currNode;
    for (unsigned int t = 0; t < img.width(); t++)
    {
      // cout<<y<<endl;
      currRow->north = prevRow;
      prevRow->south = currRow;
      if (t == img.width() - 1)
      {
        // cout<<"I made it here"<<endl;
        // cout<<y<<endl;
        // cout<<img.height()<<endl;
        if (y == (img.height() - 1))
        {
          currNode = nullptr;
          currRow = nullptr;
          prevRow = nullptr;
          head = nullptr;
          //  cout<<"I'm inside last call at constructor"<<endl;
        }
        else
        {
          // cout<<"resetting prevRow to head"<<endl;
          prevRow = head;
          head = nullptr;
        }
      }
      else
      {
        // cout<<"moving currRow and prevRow east"<<endl;
        // cout<<y<<endl;
        // cout<<currRow<<endl;
        // cout<<prevRow<<endl;
        currRow = currRow->east;
        prevRow = prevRow->east;
        // cout<<currRow<<endl;
        // cout<<prevRow<<endl;
        // cout<<"t value here is " <<t<<" and image width is " <<img.width()<<endl;
      }
    }
  }
  //cout<<"I made it to end of constructor PNG"<<endl;
  //cout<<southeast<<endl;
  // cout<<"I made it here"<<endl;
}

// while (traversal < img.width())
//       {
//         currRow->north = prevRow;
//         prevRow->south = currRow;
//         if (traversal == img.width() - 1)
//         {
//           cout<<"I made it here"<<endl;
//           if (y == img.height() - 1)
//           {
//             currNode = nullptr;
//             currRow = nullptr;
//             prevRow = nullptr;
//             head = nullptr;
//             cout<<"I made it here"<<endl;
//           }
//           else
//           {
//             prevRow = head;
//             head = nullptr;
//           }
//         }
//         else
//         {
//           currRow = currRow->east;
//           prevRow = prevRow->east;
//         }
//         traversal++;
//       }

/*
 * Copy constructor.
 * Creates this this to become a separate copy of the data in otherlist
 */
ImgList::ImgList(const ImgList &otherlist)
{
  // build the linked node structure using otherlist as a template
  Copy(otherlist);
}

/*
 * Assignment operator. Enables statements such as list1 = list2;
 *   where list1 and list2 are both variables of ImgList type.
 * POST: the contents of this list will be a physically separate copy of rhs
 */
ImgList &ImgList::operator=(const ImgList &rhs)
{
  // Re-build any existing structure using rhs as a template

  if (this != &rhs)
  { // if this list and rhs are different lists in memory
    // release all existing heap memory of this list
    Clear();

    // and then rebuild this list using rhs as a template
    Copy(rhs);
  }

  return *this;
}

/*
 * Destructor.
 * Releases any heap memory associated with this list.
 */
ImgList::~ImgList()
{
  // Ensure that any existing heap memory is deallocated
  Clear();
}

/************
 * ACCESSORS *
 ************/

/*
 * Returns the horizontal dimension of this list (counted in nodes)
 * Note that every row will contain the same number of nodes, whether or not
 *   the list has been carved.
 * We expect your solution to take linear time in the number of nodes in the
 *   x dimension.
 */
unsigned int ImgList::GetDimensionX() const
{
  if (northwest == nullptr || southeast == nullptr)
  {
    return 0;
  }
  if (northwest == southeast)
  {
    return 1;
  }
  ImgNode *initial, *next;
  unsigned int size;
  initial = northwest;
  size = 1;
  next = initial->east;
  while (next != nullptr)
  {
    next = next->east;
    initial = initial->east;
    size++;
  }
  initial = nullptr;

  return size;
}

/*
 * Returns the vertical dimension of the list (counted in nodes)
 * It is useful to know/assume that the grid will never have nodes removed
 *   from the first or last columns. The returned value will thus correspond
 *   to the height of the PNG image from which this list was constructed.
 * We expect your solution to take linear time in the number of nodes in the
 *   y dimension.
 */
unsigned int ImgList::GetDimensionY() const
{
  if (northwest == nullptr || southeast == nullptr)
  {
    return 0;
  }
  if (northwest == southeast)
  {
    return 1;
  }
  ImgNode *initial;
  unsigned int size;
  initial = northwest;
  //cout<<"The value of initial south is "<<initial->south<<endl;
  size = 1;
  while (initial->south != nullptr)
  {
    initial = initial->south;
    //cout<<"The value of initial south is "<<initial->south<<endl;
    size++;
  }
  //cout<< "value of final size is"<<size<<endl;

  return size;
}

/*
 * Returns the horizontal dimension of the list (counted in original pixels, pre-carving)
 * The returned value will thus correspond to the width of the PNG image from
 *   which this list was constructed.
 * We expect your solution to take linear time in the number of nodes in the
 *   x dimension.
 */
unsigned int ImgList::GetDimensionFullX() const
{
  if (northwest == nullptr || southeast == nullptr)
  {
    return 0;
  }

  if (northwest == southeast)
  {
    return 1;
  }
  ImgNode *initial;
  unsigned int size;
  initial = northwest;
  size = 1;
  while (initial->east != nullptr)
  {
    size += initial->skipright;
    initial = initial->east;
    size++;
  }

  return size;
}

/*
 * Returns a pointer to the node which best satisfies the specified selection criteria.
 * The first and last nodes in the row cannot be returned.
 * PRE: rowstart points to a row with at least 3 physical nodes
 * PRE: selectionmode is an integer in the range [0,1]
 * PARAM: rowstart - pointer to the first node in a row
 * PARAM: selectionmode - criterion used for choosing the node to return
 *          0: minimum luminance across row, not including extreme left or right nodes
 *          1: node with minimum total of "hue difference" with its left neighbour and with its right neighbour.
 *        In the (likely) case of multiple candidates that best match the criterion,
 *        the left-most node satisfying the criterion (excluding the row's starting node)
 *        will be returned.
 * A note about "hue difference": For PA1, consider the hue value to be a double in the range [0, 360).
 * That is, a hue value of exactly 360 should be converted to 0.
 * The hue difference is the absolute difference between two hues,
 * but be careful about differences spanning the 360 value.
 * e.g. Two pixels with hues 90 and 110 differ by 20, but
 *      two pixels with hues 5 and 355 differ by 10.
 */
ImgNode *ImgList::SelectNode(ImgNode *rowstart, int selectionmode)
{
  unsigned int rowMax = currentRowXLength(rowstart);
  if (rowMax < 3 || selectionmode < 0 || selectionmode > 1)
  {
    return nullptr;
  }
  if (rowMax == 3)
  {
    return rowstart->east;
  }
  // assumption more than 3 elements in row and selectionmode either 1 or 0
  if (selectionmode == 0)
  {
    return minLumNode(rowstart);
  }
  else
  {
    return minHueDiffNode(rowstart);
  }
}

// rowstart->east->east !=nullptr || rowstart->east!=nullptr optional usage for SelectNode

/*
 * Renders this list's pixel data to a PNG, with or without filling gaps caused by carving.
 * PRE: fillmode is an integer in the range of [0,2]
 * PARAM: fillgaps - whether or not to fill gaps caused by carving
 *          false: render one pixel per node, ignores fillmode
 *          true: render the full width of the original image,
 *                filling in missing nodes using fillmode
 * PARAM: fillmode - specifies how to fill gaps
 *          0: solid, uses the same colour as the node at the left of the gap
 *          1: solid, using the averaged values (all channels) of the nodes at the left and right of the gap
 *             Note that "average" for hue will use the closer of the angular distances,
 *             e.g. average of 10 and 350 will be 0, instead of 180.
 *             Average of diametric hue values will use the smaller of the two averages
 *             e.g. average of 30 and 210 will be 120, and not 300
 *                  average of 170 and 350 will be 80, and not 260
 *          2: *** OPTIONAL - FOR BONUS ***
 *             linear gradient between the colour (all channels) of the nodes at the left and right of the gap
 *             e.g. a gap of width 1 will be coloured with 1/2 of the difference between the left and right nodes
 *             a gap of width 2 will be coloured with 1/3 and 2/3 of the difference
 *             a gap of width 3 will be coloured with 1/4, 2/4, 3/4 of the difference, etc.
 *             Like fillmode 1, use the smaller difference interval for hue,
 *             and the smaller-valued average for diametric hues
 */


// // precondition: a and b are both in the range [0, 360].
// // postcondition: returns the angle between a and b, 
// // which must be in the range [0, 180].
// double angle_between(double a, double b) {
//   double angle1 = abs(a - b);
//   double angle2 = 360 - angle1;
//   return min(angle1, angle2);
// }

PNG ImgList::Render(bool fillgaps, int fillmode) const
{
  PNG outpng;
  if (fillmode < 0 || fillmode > 2)
  {
    return outpng;
  }
  // Add/complete your implementation below
  if (fillgaps)
  {
    outpng.resize(GetDimensionFullX(), GetDimensionY());
    // cout<<"X dimension is "<<GetDimensionFullX()<<" and Y dimension is "<<GetDimensionY()<<endl;
    return RenderGaps(fillmode, outpng);
  }
  else
  {
    outpng.resize(GetDimensionX(), GetDimensionY());
    return RenderWithoutGaps(outpng);
  }
}

PNG ImgList::RenderGaps(int fillmode, PNG &img) const
{
  // assumption fillmode is either 0 or 1
  if (fillmode == 0)
  {
    return RenderGaps0(img);
  }
  else if (fillmode == 1)
  {
    return RenderGaps1(img);
  }
  else{
    return RenderGaps2(img);
  }
}
// 0: solid, uses the same colour as the node at the left of the gap
PNG ImgList::RenderGaps0(PNG &img) const
{
  ImgNode *currRow, *nextRow;
  HSLAPixel *currentPixel;
  HSLAPixel currentColour;
  unsigned int maxX = GetDimensionFullX();
  unsigned int skipVal;
  // first point would be (0,0)
  currRow = northwest; // starts here
  for (unsigned int y = 0; y < GetDimensionY(); y++)
  {
    for (unsigned int x = 0; x < maxX; x++)
    {
      if (x == 0)
      {
        nextRow = currRow->south;
      }
      currentPixel = img.getPixel(x, y);
      currentColour = currRow->colour;
      skipVal = currRow->skipright;
      // cout<<"The current color is "<<currentColour<<" for pixel "<<currentPixel<<endl;
      setPixel(currentPixel, currentColour);
      if (skipVal > 0)
      {
        // cout<<"The current color to be fed along is "<<currentColour<<endl;
        setSameColourRow(x, y, currentColour, skipVal, img);
        x += skipVal;
      }
      currRow = currRow->east;
    }
    currRow = nextRow;
  }
  currentPixel = nullptr;
  return img;
}

/*
*          2: *** OPTIONAL - FOR BONUS ***
 *             linear gradient between the colour (all channels) of the nodes at the left and right of the gap
 *             e.g. a gap of width 1 will be coloured with 1/2 of the difference between the left and right nodes
 *             a gap of width 2 will be coloured with 1/3 and 2/3 of the difference
 *             a gap of width 3 will be coloured with 1/4, 2/4, 3/4 of the difference, etc.
 *             Like fillmode 1, use the smaller difference interval for hue,
 *             and the smaller-valued average for diametric hues
*/



PNG ImgList::RenderGaps2(PNG &img) const
{
  ImgNode *currRow, *nextRow;
  HSLAPixel *currentPixel;
  HSLAPixel colour;
  unsigned int maxX = GetDimensionFullX();
  unsigned int skipVal;
  //cout << maxX << endl;
  // first point would be (0,0)
  currRow = northwest;
  for (unsigned int y = 0; y < GetDimensionY(); y++)
  {
    for (unsigned int x = 0; x < maxX; x++)
    {
      if (x == 0)
      {
        nextRow = currRow->south;
      }
      // cout<<"The value of x here is (outside inner statement) "<<x<<endl;
      currentPixel = img.getPixel(x, y);
      // cout<<"The valye of currRow here is "<<currRow<<endl;
      // cout<<" the value the pixel is "<<currentPixel<<"at (x,y) ("<<x<<","<<y<<")"<<endl;
      // cout<<"The value of skip right is "<<currRow->skipright<<endl;
      setPixel(currentPixel, currRow->colour);
      skipVal = currRow->skipright;
      if (skipVal > 0)
      {
         double b = skipVal+1;
            b = 1/b;
         double originalB = b;
        unsigned int x1 = x;
        for(unsigned int i = 0; i < skipVal; i++){
            // send in differnece
            x1++;
            cout<<b<<endl;
            colour = differenceCalculator(b, currRow, currRow->east);
            setFillTwo(x1, y, colour, img);
            b += originalB;
        }
        x += skipVal;
      }
      // cout<<"The east to this is "<<currRow->east<<endl;
      currRow = currRow->east;
    }
    currRow = nextRow;
  }
  currentPixel = nullptr;
  return img;
}

HSLAPixel ImgList::differenceCalculator(double diff,ImgNode *leftNode, ImgNode *rightNode) const{
  HSLAPixel left, right, changed;
  left = leftNode->colour;
  right = rightNode->colour;
  changed.a = ((right.a- left.a) * diff) + left.a ;
  changed.l = ((right.l- left.l) * diff) + left.l;
  changed.s = ((right.s- left.s) * diff) + left.s;
  changed.h = hueCalculator2(left.h, right.h, diff);

  return changed;
  }


  void ImgList::setFillTwo(unsigned int x, unsigned int y, HSLAPixel colour, PNG &img) const
{
  HSLAPixel *currentPixel;
    currentPixel = img.getPixel(x, y);
    setPixel(currentPixel, colour);
  
  currentPixel = nullptr;
}





void ImgList::setSameColourRow(unsigned int x, unsigned int y, HSLAPixel colour, unsigned int skipright, PNG &img) const
{
  HSLAPixel *currentPixel;
  for (unsigned int i = 0; i < skipright; i++)
  {
    x++;
    // cout<<" the value the pixel is "<<currentPixel<<"at (x,y) ("<<x<<","<<y<<")"<<" and the color is "<<colour<<endl;
    currentPixel = img.getPixel(x, y);
    setPixel(currentPixel, colour);
  }
  currentPixel = nullptr;
}

PNG ImgList::RenderGaps1(PNG &img) const
{
  ImgNode *currRow, *nextRow;
  HSLAPixel *currentPixel;
  HSLAPixel colour;
  unsigned int maxX = GetDimensionFullX();
  unsigned int skipVal;
  //cout << maxX << endl;
  // first point would be (0,0)
  currRow = northwest;
  for (unsigned int y = 0; y < GetDimensionY(); y++)
  {
    for (unsigned int x = 0; x < maxX; x++)
    {
      if (x == 0)
      {
        nextRow = currRow->south;
      }
      // cout<<"The value of x here is (outside inner statement) "<<x<<endl;
      currentPixel = img.getPixel(x, y);
      // cout<<"The valye of currRow here is "<<currRow<<endl;
      // cout<<" the value the pixel is "<<currentPixel<<"at (x,y) ("<<x<<","<<y<<")"<<endl;
      // cout<<"The value of skip right is "<<currRow->skipright<<endl;
      setPixel(currentPixel, currRow->colour);
      skipVal = currRow->skipright;
      if (skipVal > 0)
      {
        // currRow->east is never null because absolute right row will always have skipright>0
        colour = averageColour(currRow, currRow->east);
        setSameColourRow(x, y, colour, skipVal, img);
        x += skipVal;
        // cout<<"The value of x here is Inside the loop) "<<x<<endl;
        // cout<<"The value of skip right is "<<currRow->skipright<<endl;
      }
      // cout<<"The east to this is "<<currRow->east<<endl;
      currRow = currRow->east;
    }
    currRow = nextRow;
  }
  currentPixel = nullptr;
  return img;
}

// *          1: solid, using the averaged values (all channels) of the nodes at the left and right of the gap
//  *             Note that "average" for hue will use the closer of the angular distances,
//  *             e.g. average of 10 and 350 will be 0, instead of 180.
//  *             Average of diametric hue values will use the smaller of the two averages
// diameteric
//  *             e.g. average of 30 and 210 will be 120, and not 300
//  *                  average of 170 and 350 will be 80, and not 260
HSLAPixel ImgList::averageColour(ImgNode *leftNode, ImgNode *rightNode) const
{
  // find average of both
  HSLAPixel left, right, average;
  left = leftNode->colour;
  right = rightNode->colour;
  average.a = (left.a + right.a) / 2;
  average.l = (left.l + right.l) / 2;
  average.s = (left.s + right.s) / 2;
  average.h = hueCalculator(left.h, right.h);

  return average;
}

double ImgList::hueCalculator(double leftHue, double rightHue) const
{
  double average = ((leftHue + rightHue) / 2);
  double average2;
  // double average = ((leftHue + rightHue) / 2);
   double bigger = fmax(leftHue, rightHue);
   double smaller = fmin(leftHue, rightHue);
  // double average2 = (((360 - bigger) + smaller) / 2) + bigger;
 
  // figure out angular and diamertic stuff
  if(average>=180){
    average2 = average -180;
  }
  else{
    average2= average+ 180;
  }

  if ((bigger - smaller) == 180)
  {
    return fmin(average, average2);
  }
  if(angle_between(bigger,average)< angle_between(bigger,average2)){
    return average;
  } else{
    return average2;
  }
}


double ImgList::hueCalculator2(double leftHue, double rightHue, double diff) const
{
  double val = leftHue - rightHue;
  double average = ((leftHue - val) *diff) + leftHue;
   double bigger = fmax(leftHue, rightHue);
   double smaller = fmin(leftHue, rightHue);
   double average2;

   if(average>=180){
    average2 = average -180;
  }
  else{
    average2= average+ 180;
  }

  if ((bigger - smaller) == 180)
  {
     return hueCalculator(leftHue,  rightHue);
  }

  if(angle_between(bigger,average)< angle_between(bigger,average2)){
    return average;
  } else{
    return average2;
  }
  return average;
}

PNG ImgList::RenderWithoutGaps(PNG &img) const
{
  if (GetDimensionX() == 0 || GetDimensionY() == 0)
  {
    return img;
  }
  if (northwest == southeast)
  {
    setPixel(img.getPixel(0, 0), northwest->colour);
    return img;
  }
  ImgNode *currRow, *nextRow;
  HSLAPixel *currentPixel;
  HSLAPixel colour;
  unsigned int X = currentRowXLength(northwest);
  // first point is (0,0)
  currRow = northwest;
  for (unsigned int y = 0; y < GetDimensionY(); y++)
  {
    for (unsigned int x = 0; x < X; x++)
    {
      if (x == 0)
      {
        nextRow = currRow->south;
      }
      currentPixel = img.getPixel(x, y);
      colour = currRow->colour;
      setPixel(currentPixel, colour);
      currRow = currRow->east;
    }
    currRow = nextRow;
  }
  currentPixel = nullptr;
  return img;
}

void ImgList::setPixel(HSLAPixel *pixel, HSLAPixel setter) const
{
  pixel->a = setter.a;
  pixel->s = setter.s;
  pixel->l = setter.l;
  pixel->h = setter.h;
}
/************
 * MODIFIERS *
 ************/

/*
 * Removes exactly one node from each row in this list, according to specified criteria.
 * The first and last nodes in any row cannot be carved.
 * PRE: this list has at least 3 nodes in each row
 * PRE: selectionmode is an integer in the range [0,1]
 * PARAM: selectionmode - see the documentation for the SelectNode function.
 * POST: this list has had one node removed from each row. Neighbours of the created
 *       gaps are linked appropriately, and their skip values are updated to reflect
 *       the size of the gap.
 */
void ImgList::Carve(int selectionmode)
{
  ImgNode *selected, *startRow, *nextRow;
  if (GetDimensionX() < 3 || selectionmode < 0 || selectionmode > 1)
  { // do nothing
  }
  // cout<<"Entering carve"<<endl;
  //  assume more than 3 nodes per row
  startRow = northwest;
  // cout<<"The value of start row is "<<startRow<<endl;

  // cout<<"The value of length y is "<<GetDimensionY()<<endl;
  for (unsigned int y = 0; y < GetDimensionY(); y++)
  {
    nextRow = startRow->south;
    // cout<<"The value of next row is "<<nextRow<<endl;
    selected = SelectNode(startRow, selectionmode);
    //cout<<"selected is "<<selected<<endl;
    if (selected != nullptr)
    {
      // cout<<"The value west of the chosen to be carved pixel is "<<selected->west<<endl;
      // cout<<"The value of y is "<<y<<endl;
      if (selected->north == nullptr && selected->south != nullptr)
      {
        selected->south->north = nullptr;
        selected->south->skipup += (1 + selected->skipup);
      }
      else if (selected->south == nullptr && selected->north != nullptr)
      {
        selected->north->south = nullptr;
        selected->north->skipdown += (1 + selected->skipdown);
      }
      else if (selected->south != nullptr && selected->north != nullptr)
      {
        selected->north->south = selected->south;
        selected->north->skipdown += (1 + selected->skipdown);
        selected->south->north = selected->north;
        selected->south->skipup += (1 + selected->skipup);
      }
      selected->east->west = selected->west;
      selected->east->skipleft += (1 + selected->skipleft);
      selected->west->east = selected->east;
      selected->west->skipright += (1 + selected->skipright);
      //cout<<"deleting selected: "<<selected<<endl;
      deleteNode(selected);
      startRow = nextRow;
      // cout<<"The value of start row is "<<startRow<<endl;
    }
  }

  // add your implementation here
}

// note that a node on the boundary will never be selected for removal
/*
 * Removes "rounds" number of nodes (up to a maximum of node width - 2) from each row,
 * based on specific selection criteria.
 * Note that this should remove one node from every row, repeated "rounds" times,
 * and NOT remove "rounds" nodes from one row before processing the next row.
 * PRE: selectionmode is an integer in the range [0,1]
 * PARAM: rounds - number of nodes to remove from each row
 *        If rounds exceeds node width - 2, then remove only node width - 2 nodes from each row.
 *        i.e. Ensure that the final list has at least two nodes in each row.
 * POST: this list has had "rounds" nodes removed from each row. Neighbours of the created
 *       gaps are linked appropriately, and their skip values are updated to reflect
 *       the size of the gap.
 */
void ImgList::Carve(unsigned int rounds, int selectionmode)
{
  if (rounds > GetDimensionX() - 2)
  {
    rounds = GetDimensionX() - 2;
  }
  for (unsigned int r = 0; r < rounds; r++)
  {
    Carve(selectionmode);
  }
  // add your implementation here
}

/*
 * Helper function deallocates all heap memory associated with this list,
 * puts this list into an "empty" state. Don't forget to set your member attributes!
 * POST: this list has no currently allocated nor leaking heap memory,
 *       member attributes have values consistent with an empty list.
 */
void ImgList::Clear()
{
  //cout<<northwest<<endl;
  //cout<<southeast<<endl;
  // cout<<"I'm at clear"<<endl;
  if (northwest == nullptr || southeast == nullptr)
  {
    return;
  }
  ImgNode *currRow, *nextRow;
  unsigned int maxY = GetDimensionY();
  //cout<<"val of macY is "<<maxY<<endl;
  // for when y = 0
      currRow = northwest;
      nextRow = currRow->south;
  // if (maxY == 1)
  //   {
  //     deleteRow(currRow);
  //   }
    //cout<<" I made it past clear"<<endl;
      
  for (unsigned int y = 0; y < maxY; y++)
  {
    // assume more than 1 row
  
    deleteRow(currRow);
    currRow = nextRow;
    if (nextRow == nullptr)
    {
    }
    else
    {
      nextRow = currRow->south;
    }
  }
  northwest = nullptr;
  southeast = nullptr;
}

/* ************************
 *  * OPTIONAL - FOR BONUS *
 ** ************************
 * Helper function copies the contents of otherlist and sets this list's attributes appropriately
 * PRE: this list is empty
 * PARAM: otherlist - list whose contents will be copied
 * POST: this list has contents copied from by physically separate from otherlist
 */
void ImgList::Copy(const ImgList &otherlist)
{
  // add your implementation here
  if(GetDimensionX()==0||GetDimensionY()==0){
    // build the linked node structure and set the member attributes appropriately
  if (otherlist.GetDimensionX() <= 0 || otherlist.GetDimensionY() <= 0)
  {
    this->northwest = nullptr;
    this->southeast = nullptr;
    return;
  }

  ImgNode *currNode, *head, *currRow, *prevRow, *otherlistStart;
  this->northwest = nullptr;
  this->southeast = nullptr;
  currNode = makeCopyRow(0, otherlist,otherlist.northwest);
  prevRow = currNode;
  otherlistStart = otherlist.northwest->south;
  // need to update this

  for (unsigned int y = 1; y < otherlist.GetDimensionY(); y++)
  {
    currNode = makeCopyRow(y, otherlist,otherlistStart);
    currRow = currNode;
    head = currNode;
    for (unsigned int t = 0; t < otherlist.GetDimensionX(); t++)
    {
      currRow->north = prevRow;
      prevRow->south = currRow;
      if (t == otherlist.GetDimensionX() - 1)
      {
        if (y == (otherlist.GetDimensionY() - 1))
        {
          currNode = nullptr;
          currRow = nullptr;
          prevRow = nullptr;
          head = nullptr;
        }
        else
        {
          otherlistStart = otherlistStart->south;
          prevRow = head;
          head = nullptr;
        }
      }
      else
      {
        currRow = currRow->east;
        prevRow = prevRow->east;
      }
    }
  }
  }
  // else don nothing because list is not empty
}

/*************************************************************************************************
 * IF YOU DEFINED YOUR OWN PRIVATE FUNCTIONS IN imglist.h, YOU MAY ADD YOUR IMPLEMENTATIONS BELOW *
 *************************************************************************************************/
ImgNode *ImgList::makeCopyRow(unsigned int y, const ImgList &otherlist, ImgNode* otherlistStart)
{
  // fixed it
  ImgNode *currNode, *prevNode, *head, *otherlistCurrNode;
  otherlistCurrNode = otherlistStart;
  currNode = copyNode(otherlistCurrNode);
  //cout<<"I made it to start of make Row"<<endl;
  if (this->northwest == nullptr)
      {
        this->northwest = currNode;
      }
      // as x ==0 here
      prevNode = currNode;
      head = currNode;
      if (y == (otherlist.GetDimensionY()-1) && 0 == (otherlist.GetDimensionX() - 1))
      {
        this->southeast = currNode;
      }
    
  for (unsigned int x = 1; x < otherlist.GetDimensionX(); x++)
  {
    otherlistCurrNode = otherlistCurrNode->east;
    currNode = copyNode(otherlistCurrNode);
      if (y == (otherlist.GetDimensionY() - 1) && x == (otherlist.GetDimensionX() - 1))
      {
        southeast = currNode;
      }
      prevNode->east = currNode;
      currNode->west = prevNode;
      prevNode = currNode;
  }
  currNode = nullptr;
  prevNode = nullptr;
  //cout<<"I made it to end of makeRow"<<endl;
  return head;
}

ImgNode *ImgList::copyNode(ImgNode* othernode)
{
  cout<<"copying node "<<othernode<<endl;
  ImgNode *node = new ImgNode();
  cout<<"new node "<<node<<endl;
  node->colour = othernode->colour;
  node->skipdown= othernode->skipdown;
  node->skipup= othernode->skipup;
  node->skipright=othernode->skipright;
  node->skipleft=othernode->skipleft;
   //cout<<"Creating node "<<node<<endl;
  return node;
}

ImgNode *ImgList::makeRow(unsigned y, PNG &img)
{
  // fixed it
  ImgNode *currNode, *prevNode, *head;
  currNode = createNode(img.getPixel(0, y));
  //cout<<"I made it to start of make Row"<<endl;
  if (northwest == nullptr)
      {
        northwest = currNode;
      }
      // as x ==0 here
      prevNode = currNode;
      head = currNode;
      if (y == (img.height()-1) && 0 == (img.width() - 1))
      {
        southeast = currNode;
      }
    
  for (unsigned int x = 1; x < img.width(); x++)
  {
    currNode = createNode(img.getPixel(x, y));
      if (y == (img.height() - 1) && x == (img.width() - 1))
      {
        southeast = currNode;
      }
      prevNode->east = currNode;
      currNode->west = prevNode;
      prevNode = currNode;
  }
  currNode = nullptr;
  prevNode = nullptr;
  //cout<<"I made it to end of makeRow"<<endl;
  return head;
}

ImgNode *ImgList::createNode(HSLAPixel *currentPixel)
{
  ImgNode *node = new ImgNode();
  node->colour = *currentPixel;
   //cout<<"Creating node "<<node<<endl;
  return node;
}

ImgNode *ImgList::minLumNode(ImgNode *rowstart)
{
  // this assumes there are at least 3+ nodes in the row
  ImgNode *currNode, *lowest;
  double lum = 1;
  currNode = rowstart->east;
  lowest = nullptr;
  double currentlum;
  for (unsigned int x = 1; x < currentRowXLength(rowstart) - 1; x++)
  {
    currentlum = currNode->colour.l;
    //cout<<"currentLum is "<<currentlum<<" of node "<< currNode<<endl;
    if (currentlum < lum)
    {
      lowest = currNode;
      lum = currentlum;
      //cout<<" lum is now "<<lum<<endl;
      //cout<<"Lowest is "<<lowest<<" with lum val "<<lowest->colour.l<<endl;
    }
    currNode = currNode->east;
  }
   //cout<<"Final lum is now "<<lum<<endl;
      //cout<<"Final Lowest is "<<lowest<<" with lum val "<<lowest->colour.l<<endl;
  currNode = nullptr;
  return lowest;
}

ImgNode *ImgList::minHueDiffNode(ImgNode *rowstart)
{
  // this assumes there are at least 3+ nodes in the row
  ImgNode *currNode, *lowest;
  double hue = 360;
  currNode = rowstart->east;
  lowest = nullptr;
  double leftHuediff, rightHuediff, total;
  for (unsigned int x = 1; x < currentRowXLength(rowstart) - 1; x++)
  {
    // cout<<"The lum value for current node at postion "<<x<<" is "<<lum<<endl;
    // cout<<"The value for X is "<<GetDimensionX()<<endl;
    // cout<<"The value for X is "<<currentRowXLength(rowstart)<<endl;

    leftHuediff = HueDiff(currNode->west->colour.h, currNode->colour.h);
    rightHuediff = HueDiff(currNode->east->colour.h, currNode->colour.h);
    total = leftHuediff + rightHuediff;

    if (total < hue)
    {
      lowest = currNode;
      hue = total;
      // cout<<"Inside loop lowest variable value is "<<lowest->colour.l<<endl;
      // cout<<"Inside loop lowest lum value is "<<lum<<endl;
    }
    currNode = currNode->east;
  }
  currNode = nullptr;
  // cout<<"The lowest lum val is "<<lowest->colour.l<<endl;
  return lowest;
}

void ImgList::deleteNode(ImgNode *node)
{
   cout<<"Deleting node "<<node<<endl;
  delete node;
  node = nullptr;
}

void ImgList::deleteRow(ImgNode *start)
{
  ImgNode *currNode, *nextNode;
  currNode = start;
  //cout<<"I made it to delte Row initial"<<endl;
  unsigned int maxX = currentRowXLength(start);
    //cout<<"The max length of this row is "<<maxX<<endl;

  for (unsigned int x = 0; x < maxX; x++)
  {
      nextNode = currNode->east;
      deleteNode(currNode);
      currNode = nextNode;
    
  }
}

unsigned int ImgList::currentRowXLength(ImgNode *current) const
{
  ImgNode *initial, *next;
  unsigned int size;
  initial = current;
  size = 1;
  next = initial->east;
  while (next != nullptr)
  {
    next = next->east;
    initial = initial->east;
    size++;
  }
  initial = nullptr;

  return size;
}

double ImgList::angle_between(double a, double b) const {
  double angle1 = abs(a - b);
  double angle2 = 360 - angle1;
  return min(angle1, angle2);
}