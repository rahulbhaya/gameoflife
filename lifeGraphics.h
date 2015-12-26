/*
 * File: lifeGraphics.h
 * Modified Fri Sep 29 12:30:59 PDT 2006 jzelenski
 * -----------------------------------------------
 * Defines simple drawing routines for a 2-d grid of circles,
 * designed for use in the Life assignment.
 */

#ifndef _lifegraphics_h
#define _lifegraphics_h

/*
 * Constant: MaxAge
 * ----------------
 * We only have a fixed number of density levels to show
 * the progression of age. At age 1, a cell is a solid dot,
 * and each subsequent age is fainter. At age MaxAge and beyond, 
 * the same very faint pattern is used.
 */
const int MaxAge = 12;


/*
 * Function: InitLifeGraphics
 * Usage: InitLifeGraphics(10, 25);
 * --------------------------------
 * This will erase the graphics window completely and draw a black
 * border around the simulation rectangle which is centered in the
 * window.  The grid cells will be sized as large as will fit given
 * the grid geometry. Grids with more rows and columns will use smaller 
 * cells and vice versa. This function can be used at the beginning of 
 * a simulation or between generations to clear the window before drawing 
 * the next generation.
 */
void InitLifeGraphics(int numRows, int numCols);


/*
 * Function: DrawCellAt
 * Usage: DrawCellAt(5, 0, 2);
 * ---------------------------
 * Draws the cell at the specific row and col, replacing any previous
 * cell at that location.  The grid geometry must have earlier been 
 * configured with the InitLifeGraphics function above. Rows and columns 
 * are specified using zero-based indexing and (0,0) is the upper-left corner.  
 * If the location given is not in bounds, an error is raised.
 * At startup, a random color is chosen for the simulation.  Each cell 
 * will be displayed in a shade which tells its age.  Cells that have just 
 * been born (i.e. that have value 1) are the darkest, they get lighter with 
 * age as the values go to 2, 3, and so on.  The cells stabilize as very faint 
 * at generation MaxAge and older.  Passing 0 for age means the cell is not alive,
 * the cell is drawn in white, erasing any previous contents.
 */
void DrawCellAt(int row, int col, int age);



#endif 

