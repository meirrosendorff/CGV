
/**
 * CPSC 424, Fall 2015, Lab 6:  Light and Material in OpenGL 1.1.
 * This program shows a square "stage" with a variety of objects
 * arranged on it.  The objects use several shapes and materials
 * and include a wireframe object that is drawn with lighting
 * turned off.
 *
 * This program use GLU as well as GLUT, and it depends on polyhedron.c, 
 * which requires the math library.  It can be compiled with
 *
 *        gcc -o lab6 lab6.c polyhedron.c -lGL -lglut -lGLU -lm
 */

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <stdio.h>      // (Can be used for debugging messages, with printf().)
#include "polyhedron.h" // For access to the regular polyhedra from polyhedron.c.

// --------------------------- Data for some materials ---------------------------------------------------

int vertical = 0; 
int horizontal = 0; 

typedef enum {false, true} bool;

/**
 * One of the rows of this array corresponds to a set of material properties.  Items 0 to 4 in a row
 * specify an ambient color; items 4 through 7, a diffuse color; items 8 through 11, a specular color;
 * and item 12, a specular exponent (shininess value).  The data is adapted from the table on the page
 * http://devernay.free.fr/cours/opengl/materials.html
 */
float materials[][13] = {
	{ /* "emerald" */   0.0215f, 0.1745f, 0.0215f, 1.0f, 0.07568f, 0.61424f, 0.07568f, 1.0f, 0.633f, 0.727811f, 0.633f, 1.0f, 0.6f*128 },
	{ /* "jade" */   0.135f, 0.2225f, 0.1575f, 1.0f, 0.54f, 0.89f, 0.63f, 1.0f, 0.316228f, 0.316228f, 0.316228f, 1.0f, 0.1f*128 },
	{ /* "obsidian" */   0.05375f, 0.05f, 0.06625f, 1.0f, 0.18275f, 0.17f, 0.22525f, 1.0f, 0.332741f, 0.328634f, 0.346435f, 1.0f, 0.3f*128 },
	{ /* "pearl" */   0.25f, 0.20725f, 0.20725f, 1.0f, 1.0f, 0.829f, 0.829f, 1.0f, 0.296648f, 0.296648f, 0.296648f, 1.0f, 0.088f*128 },
	{ /* "ruby" */   0.1745f, 0.01175f, 0.01175f, 1.0f, 0.61424f, 0.04136f, 0.04136f, 1.0f, 0.727811f, 0.626959f, 0.626959f, 1.0f, 0.6f*128 },
	{ /* "turquoise" */   0.1f, 0.18725f, 0.1745f, 1.0f, 0.396f, 0.74151f, 0.69102f, 1.0f, 0.297254f, 0.30829f, 0.306678f, 1.0f, 0.1f*128 },
	{ /* "brass" */   0.329412f, 0.223529f, 0.027451f, 1.0f, 0.780392f, 0.568627f, 0.113725f, 1.0f, 0.992157f, 0.941176f, 0.807843f, 1.0f, 0.21794872f*128 },
	{ /* "bronze" */   0.2125f, 0.1275f, 0.054f, 1.0f, 0.714f, 0.4284f, 0.18144f, 1.0f, 0.393548f, 0.271906f, 0.166721f, 1.0f, 0.2f*128 },
	{ /* "chrome" */   0.25f, 0.25f, 0.25f, 1.0f, 0.4f, 0.4f, 0.4f, 1.0f, 0.774597f, 0.774597f, 0.774597f, 1.0f, 0.6f*128 },
	{ /* "copper" */   0.19125f, 0.0735f, 0.0225f, 1.0f, 0.7038f, 0.27048f, 0.0828f, 1.0f, 0.256777f, 0.137622f, 0.086014f, 1.0f, 0.1f*128 },
	{ /* "gold" */   0.24725f, 0.1995f, 0.0745f, 1.0f, 0.75164f, 0.60648f, 0.22648f, 1.0f, 0.628281f, 0.555802f, 0.366065f, 1.0f, 0.4f*128 },
	{ /* "silver" */   0.19225f, 0.19225f, 0.19225f, 1.0f, 0.50754f, 0.50754f, 0.50754f, 1.0f, 0.508273f, 0.508273f, 0.508273f, 1.0f, 0.4f*128 },
	{ /* "cyan plastic" */   0.0f, 0.1f, 0.06f, 1.0f, 0.0f, 0.50980392f, 0.50980392f, 1.0f, 0.50196078f, 0.50196078f, 0.50196078f, 1.0f, .25f*128 },
	{ /* "green plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.35f, 0.1f, 1.0f, 0.45f, 0.55f, 0.45f, 1.0f, .25f*128 },
	{ /* "red plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.6f, 0.6f, 1.0f, .25f*128 },
	{ /* "cyan rubber" */   0.0f, 0.05f, 0.05f, 1.0f, 0.4f, 0.5f, 0.5f, 1.0f, 0.04f, 0.7f, 0.7f, 1.0f, .078125f*128 },
	{ /* "green rubber" */   0.0f, 0.05f, 0.0f, 1.0f, 0.4f, 0.5f, 0.4f, 1.0f, 0.04f, 0.7f, 0.04f, 1.0f, .078125f*128 },
	{ /* "red rubber" */   0.05f, 0.0f, 0.0f, 1.0f, 0.5f, 0.4f, 0.4f, 1.0f, 0.7f, 0.04f, 0.04f, 1.0f, .078125f*128 },
};


// ------------------------ OpenGL rendering and  initialization -----------------------

/**
 * The display method is called when the panel needs to be drawn.
 * Here, it draws a stage and some objects on the stage.
 */


void setMaterial(int index){

	//partitions the chosen material and sets those properties
	float properties[3][4];
	float shine = materials[index][12];
	for (int i = 0; i < 3; i++){
		for ( int j = 0; j < 4; j++){
		
			properties[i][j] = materials[index][4*i+j];
		}
	}
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, properties[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, properties[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, properties[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, properties[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shine);
	
	
	
}
 
void drawPoly(struct Polyhedron poly, bool solid){
	//draws the given polyhedra
	int j = -1;
	
	for (int i = 0; i < poly.faceCount; i++){ //iterate over the faces
		
		if (solid){
			glBegin(GL_TRIANGLE_FAN); //if solid use a triangle fan
		}else{
			glBegin(GL_LINE_LOOP);	//if not solid
			glDisable(GL_LIGHTING);	//disable lighting
		}
		
		//set the face colour
		if (poly.faceColors != NULL){
			int r = poly.faceColors[3*i];
			int g = poly.faceColors[3*i + 1];
			int b = poly.faceColors[3*i + 2];
			glColor3f(r,g,b);
		}
		
		//set the normals
		if (poly.normals != NULL){
			int x = poly.normals[3*i];
			int y = poly.normals[3*i + 1];
			int z = poly.normals[3*i + 2];
			glNormal3f(x,y,z);
		}
		
		
		while(poly.faces[++j] != -1){ //draw each face
		
			float x = poly.vertices[poly.faces[j] * 3];
			float y = poly.vertices[poly.faces[j] * 3 + 1];
			float z = poly.vertices[poly.faces[j] * 3 + 2];
			glVertex3f(x,y,z);
		}
		
		glEnd();
		
		if(!solid) glEnable(GL_LIGHTING);
	
	}

}
 
 
void display() {
        // called whenever the display needs to be redrawn

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();
    gluLookAt( 0,8,40, 0,1,0, 0,1,0 );  // viewing transform	
    
    glPushMatrix();
    glRotatef(vertical, 1, 0, 0);
    glRotatef(horizontal, 0, 1, 0);
    float light1Pos[4] = {7, 0, 7, 1};
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    
    float light2Pos[4] = {-7.5, 0, 9, 1};
    glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
    
    float gray[] = { 0.6f, 0.6f, 0.6f, 1 };
    float zero[] = { 0, 0, 0, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    glPushMatrix();
    glTranslatef(0,-1.5,0); // Move top of stage down to y = 0
    glScalef(1, 0.05, 1); // Stage will be one unit thick,
    glutSolidCube(20);
    glPopMatrix();

    // TODO draw some shapes!
    
    float white[] = {1.0f,1.0f,1.0f,1};
    float blue[] = {0,0,1,1};
    
    
   glPushMatrix();//wireTeapot
    	glColor3f(1,0,0);
    	glDisable(GL_LIGHTING);
    	glTranslatef(0, 1, 0);
    	glutWireTeapot(3);
    	glEnable(GL_LIGHTING);
    glPopMatrix();
    
    glPushMatrix();//Normal teapot
		setMaterial(5);
    	glTranslatef(0, 0.99, 0);
    	glScalef(1.029,1,1);
    	glutSolidTeapot(2.9);
    glPopMatrix();
    
    
    glPushMatrix();//Draw cone
    	setMaterial(8);
    	glTranslatef(7.5,-1,7.5);
    	glRotatef(-90, 1,0,0);
    	glutSolidCone(2,4,30,8);
    	
    glPopMatrix();
    
    glPushMatrix();//Draw box around cone
    	glLineWidth(10);
    	setMaterial(7);
    	glTranslatef(7.5,0.5,7.5);
    	glScalef(1,1.5,1);
    	glutWireCube(3);
    	glLineWidth(1);
    glPopMatrix();
    
    
    glPushMatrix(); // draw a cube
		glTranslatef(0,0,7);
		setMaterial(0);
		drawPoly(cube, 1);
    glPopMatrix();
     
    glPushMatrix(); // draw a wire octahedron
		glTranslatef(-8,0,9);
		setMaterial(10);
		drawPoly(octahedron, 0);
    
    glPopMatrix();     
     
    glPopMatrix();
    glutSwapBuffers();  // (Required for double-buffered drawing, at the end of display().)
}

/**
 * initGL() is called just once, by main(), to do initialization of OpenGL state
 * and other global state. Here, it sets up a projection, configures some lighting, 
 * and enables the depth test.  It also calls createPolyhedra(), whcih is defined
 * in the included file, polyhedron.h.
 */
void initGL() {
    createPolyhedra();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20, 2, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
   
   	//Create a red Light
     
    float red1[4] = {0.6, 0.4, 0.4, 1}; //diffuse color
    float red2[4] = { 0.8, 0, 0, 1}; //ambient color
    float red3[4] = {0.15,0, 0,  1}; // speculare colour
    float pos[4] = {-9,2,0,1};		//position
    glLightfv(GL_LIGHT1, GL_DIFFUSE,red1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, red2);
    glLightfv(GL_LIGHT1 ,GL_SPECULAR, red3);
    glLightfv(GL_LIGHT1 ,GL_POSITION, pos);
    
}  // end initGL()


// ------------------------------ mouse handling functions ----------------------------------

int dragging;        // 0 or 1 to indicate whether a drag operation is in progress
int dragButton;      // which button started the drag operation
int prevX, prevY;    // previous mouse position during drag

/*  mouseUpOrDown() is set up in main() to be called when the user presses or releases
 *  a button on the mouse.  The button paramter is one of the contants GLUT_LEFT_BUTTON,
 *  GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.  The buttonState is GLUT_UP or GLUT_DOWN and
 *  tells whether this is a mouse press or a mouse release event.  x and y give the
 *  mouse position in pixel coordinates, with (0,0) at the UPPER LEFT.
 */
void mouseUpOrDown(int button, int buttonState, int x, int y) {
       // called to respond to mouse press and mouse release events
   if (buttonState == GLUT_DOWN) {  // a mouse button was pressed
       if (dragging)
          return;  // Ignore a second button press during a draw operation.
       dragging = 1;
       dragButton = button;
       prevX = x;
       prevY = y;
   }
   else {  // a mouse button was released
       if ( ! dragging || button != dragButton )
           return; // this mouse release does not end a drag operation.
       dragging = 0;
   }
}

/*  mouseDragged() is set up in main() to be called when the user moves the mouse,
 *  but only when one or more mouse buttons are pressed.  x and y give the position
 *  of the mouse in pixel coordinates.
 */
void mouseDragged(int x, int y) {
        // called to respond when the mouse moves during a drag
    if ( ! dragging )
        return;
    // TODO Do something when the mouse moves!
    
	int temp = (vertical + (y - prevY)/5);
	if (temp > -25 && temp < 25) vertical = temp; //rotate on the x axis 

    
    horizontal = (horizontal + (x - prevX)/5); //rotate on the y axis
    
    glutPostRedisplay();  // make OpenGL redraw the scene
    prevX = x;
    prevY = y;
}


// ----------------- main routine -------------------------------------------------

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Allows processing of certain GLUT command line options
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);  // Use double buffering and a depth buffer.
    glutInitWindowSize(1000,500);       // size of display area, in pixels
    glutInitWindowPosition(100,100);    // location in window coordinates
    glutCreateWindow("Stage");          // parameter is window title  
    initGL();                           // do OpenGL initialization for the window
    glutDisplayFunc(display);           // call display() to draw the scene
    glutMouseFunc(mouseUpOrDown);       // call mouseUpOrDown() for mousedown and mouseup events
    glutMotionFunc(mouseDragged);       // call mouseDragged() when mouse moves, only during a drag gesture
    glutMainLoop(); // Run the event loop!  This function does not return.
    return 0;
}

