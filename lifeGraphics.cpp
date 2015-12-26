/*
 * File: lifeGraphics.cpp
 * Last modified on Sun Oct  1 18:01:39 PDT 2006 by jzelenski
 * ----------------------------------------------------------
 * Implementation of the Life graphics support routines.  We have some
 * internal private helpers that do calculations for cell-size, 
 * colors, and "aging" the cells using shading. You are
 * welcome to read over this code, but you will not need to edit it unless 
 * you're attempting some sort of fancy extension.
 */
 
#include "lifeGraphics.h"
#include "genlib.h"
#include "extgraph.h"
#include "random.h"
#include "strutils.h"

const double PIXELS_IN_INCH = 72.0;		// 72 dpi is standard
const double ONE_PIXEL = (1.0/PIXELS_IN_INCH);

// private module data
static struct {
	double upperLeftX, upperLeftY;
	double cellDiameter;
	int numRows, numCols;
} curSim;


static void CalculateGeometry(int numRows, int numCols, double&x, double& y, double& diameter);
static void DrawFilledCircle(double x, double y, double radius, double fill, string color);
static void DrawBox(double x, double y, double w, double h);
static double ScaleForGeneration(double first, double last, int generation);
static double BrightnessForGeneration(int generation);
static double FillForGeneration(int generation);
static void InitColors();

#define MIN(x, y) ((x) < (y) ? (x) : (y))



void InitLifeGraphics(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0)
		Error("InitLifeGraphics(): number of rows and columns must both be positive!"); 
	curSim.numRows = nRows;	// store nRows & nCols so we can later verify cells are in bounds
	curSim.numCols = nCols;
	InitGraphics();						// Erase entire window
    InitColors();
	CalculateGeometry(nRows, nCols, curSim.upperLeftX, curSim.upperLeftY, curSim.cellDiameter);
	SetPenColor("Black");
	// draw black border around grid, outdented by one pixel
	DrawBox(curSim.upperLeftX - ONE_PIXEL, curSim.upperLeftY + ONE_PIXEL, 
			curSim.numCols*curSim.cellDiameter + 2*ONE_PIXEL, 
			curSim.numRows*curSim.cellDiameter + 2*ONE_PIXEL);
}



void DrawCellAt(int row, int col, int value)
{
	if (row < 0 || row >= curSim.numRows || col < 0 || col >= curSim.numCols)
		Error(string(__FUNCTION__) + " asked to draw location (" + IntegerToString(row) + ", " + IntegerToString(col) + ") which is outside the bounds of the current simulation."); 
		
	value  = MIN(value, MaxAge);
	string color = "Color" + IntegerToString(value);
	double fill = FillForGeneration(value);
    DrawFilledCircle(curSim.upperLeftX  + (col+.5)*curSim.cellDiameter,	// compute center of cell
					  curSim.upperLeftY - (row+.5)*curSim.cellDiameter, 
					  curSim.cellDiameter/2.0 - ONE_PIXEL,  	// inset radius slightly, so cells don't touch
					  fill, color);
}



/* CalculateGeometry
 * -----------------
 * Calculates maximum size of cells that will fit in window by finding
 * which is more constrainted (width/height) and setting cell diameter
 * appropriately. We work in pixels to make sure we have an integral
 * number of pixels for decent rendering.
 * This function also calculates the upperLeft X and Y so that the 
 * grid is centered in the graphics window.
 */
static void CalculateGeometry(int numRows, int numCols, double& x, double& y, double& diameter)
{
	int hPixelsPerCell = (int) (GetWindowHeight()*PIXELS_IN_INCH)/numRows;
	int wPixelsPerCell = (int) (GetWindowWidth() *PIXELS_IN_INCH)/numCols;
	
	// Choose the smaller (which is more constrained) then divide 
	// by 72 for points to inch conversion
	diameter = (MIN(wPixelsPerCell, hPixelsPerCell))/PIXELS_IN_INCH;

	// Center grid in window for pleasing effect
	x = (GetWindowWidth() - (numCols*diameter))/2;
	y = GetWindowHeight() - (GetWindowHeight() - numRows*diameter)/2;
	// subtract from window height because graphics coordinate system grows UP!
}




/* These functions handle all the color/density changes for cells as they age.
 * Start newly born cells with a low brightness, high density color choice,
 * such as a deep red or blue, close to black. In a b&w world, this will be black
 * As generations go by, increase the brightness and decrease the density.
 * So cells will become brighter red and the added white will exhibit pastel
 * tone (or gray in b&w). For final generation, cells are totally bright and very
 * low density.  Choose a random starting color for base color and adjust for
 * correct brightness/density values by scaling between the desired first & last 
 * values.  Register the colors with the graphics library using DefineColor() under
 * names "Color1" (for first generation), "Color2" (for second), and so on.
 */

static double ScaleForGeneration(double first, double last, int generation)
{
	return first + generation*((last-first)/MaxAge);
}


const double FIRST_FILL = 1.0;	// first generations are full-density
const double LAST_FILL = .15;	// later generations are low-density

static double FillForGeneration(int generation)
{
	return ScaleForGeneration(FIRST_FILL, LAST_FILL, generation);
}


const double FIRST_BRIGHTNESS = .60;	// first generations are low-brightness
const double LAST_BRIGHTNESS = 1.0;		// later generations are full brightness

static double BrightnessForGeneration(int generation)
{
	return ScaleForGeneration(FIRST_BRIGHTNESS, LAST_BRIGHTNESS, generation);
}


// Picks a random starting color and then sets up the Color1-Color12 names that
// map to the various levels of brightness for different ages.
static void InitColors()
{
	static bool beenHere = false;	// this function will execute at most once per run of the program
	if (beenHere) return;
	beenHere = true;
    double red = RandomReal(0,1);	// choose a random starting color
    double blue = RandomReal(0,1);
    double green = RandomReal(0,1);
    DefineColor("Color0", 1.0, 1.0, 1.0); // Age 0 is color white
	for (int i = 1; i <= MaxAge; i++) {
    	double brightness = BrightnessForGeneration(i);
		DefineColor("Color" + IntegerToString(i), brightness*red, brightness*green, brightness*blue);
	}
}



/* (x, y) specifies the center of the circle. 
 */ 
static void DrawFilledCircle(double x, double y, double radius, double fill, string color)
{
	SetPenColor(color);
	MovePen(x + radius, y);  // start at right side of circle (0 degree Cartesian)
	StartFilledRegion(fill);
	DrawArc(radius, 0, 360);
	EndFilledRegion();
}


/* This version of drawBox is slightly odd in that the anchor point is
 * the upper left corner, rather than the expected lower left.  It
 * happened to make things slightly more convenient in this case, and since
 * DrawBox is used only in this one way,  I went with this non-standard design.
 */
static void DrawBox(double upperLeftX, double upperLeftY, double width, double height)
{
	MovePen(upperLeftX, upperLeftY);
	DrawLine(width, 0);
	DrawLine(0, -height);
	DrawLine(-width, 0);
	DrawLine(0, height);
}

