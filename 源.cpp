////#include <windows.h>
//#include <afx.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
//#include <gl/glut.h>
//#include <math.h>
//
////#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
//
//#define glRGB(x, y, z)  glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
//
//#define SUN         1           // 太阳
//#define MERCURY     2           // 水星
//#define VENUS       3           // 金星
//#define EARTH       4           // 地球
//#define MARS        5           // 火星
//#define JUPITER     6           // 木星
//#define SATURN      7           // 土星
//#define URANUS      8           // 天王星
//#define NEPTUNE     9           // 海王星
//#define PLUTO       10          // 冥王星
//
//
//GLfloat fAspect;
//
//// Called to draw scene
//void RenderScene(GLenum mode)
//{
//	//此处可加gluLookAt()函数
//
//	// Save the matrix state and do the rotations
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//
//	// Translate the whole scene out and into view
//	glTranslatef(0.0f, 0.0f, -300.0f);
//
//	// Initialize the names stack
//	glInitNames();
//	glPushName(0);
//
//	// Set material color, Yellow
//	// Draw Sun
//	glRGB(255, 255, 0);
//	if (mode == GL_SELECT)
//		glLoadName(SUN);
//	glutSolidSphere(15.0f, 15, 15);
//
//	// Draw Mercury
//	glRGB(128, 0, 0);
//	glPushMatrix();
//	glTranslatef(20.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(MERCURY);
//	glutSolidSphere(2.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw Venus
//	glPushMatrix();
//	glRGB(128, 128, 255);
//	glTranslatef(30.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(VENUS);
//	glutSolidSphere(4.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw the Earth
//	glPushMatrix();
//	glRGB(0, 0, 255);
//	glTranslatef(50.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(EARTH);
//	glutSolidSphere(8.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw Mars
//	glRGB(255, 0, 0);
//	glPushMatrix();
//	glTranslatef(65.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(MARS);
//	glutSolidSphere(4.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw Jupiter
//	glRGB(238, 216, 174);
//	glPushMatrix();
//	glTranslatef(80.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(JUPITER);
//	glutSolidSphere(10.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw Saturn
//	glRGB(255, 192, 203);
//	glPushMatrix();
//	glTranslatef(105.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(SATURN);
//	glutSolidSphere(10.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw Uranus
//	glRGB(173, 216, 230);
//	glPushMatrix();
//	glTranslatef(130.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(URANUS);
//	glutSolidSphere(12.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw Neptune
//	glRGB(30, 144, 255);
//	glPushMatrix();
//	glTranslatef(160.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(NEPTUNE);
//	glutSolidSphere(8.0f, 15, 15);
//	glPopMatrix();
//
//	// Draw Pluto
//	glRGB(238, 174, 238);
//	glPushMatrix();
//	glTranslatef(190.0f, 0.0f, 0.0f);
//	if (mode == GL_SELECT)
//		glLoadName(PLUTO);
//	glutSolidSphere(12.0f, 15, 15);
//	glPopMatrix();
//
//	// Restore the matrix state
//	glPopMatrix();
//	// Modelview matrix
//	glutSwapBuffers();
//	// redraw...
//	glutPostRedisplay();
//}
//
//void Display(void)
//{
//	// Clear the window with current clearing color
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	RenderScene(GL_RENDER);
//}
//
//// Present the information on which planet/sun was selected and displayed
//void ProcessPlanet(GLuint id)
//{
//	switch (id)
//	{
//	case SUN:
//		MessageBox(NULL, _T("You clicked on the Sun!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case MERCURY:
//		MessageBox(NULL, _T("You clicked on Mercury!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case VENUS:
//		MessageBox(NULL, _T("You clicked on Venus!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case EARTH:
//		MessageBox(NULL, _T("You clicked on Earth!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case MARS:
//		MessageBox(NULL, _T("You clicked on Mars!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case JUPITER:
//		MessageBox(NULL, _T("You clicked on Jupiter!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case SATURN:
//		MessageBox(NULL, _T("You clicked on Saturn!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case URANUS:
//		MessageBox(NULL, _T("You clicked on Uranus!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case NEPTUNE:
//		MessageBox(NULL, _T("You clicked on Neptune!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	case PLUTO:
//		MessageBox(NULL, _T("You clicked on Pluto!"), _T("Info"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//
//	default:
//		MessageBox(NULL, _T("Nothing was clicked on!"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
//		break;
//	}
//}
//
//// Process the selection, which is triggered by a right mouse
//// click at (xPos, yPos).
//#define BUFFER_LENGTH 64
//void ProcessSelection(int xPos, int yPos)
//{
//	// Space for selection buffer
//	GLuint selectBuff[BUFFER_LENGTH];
//
//	// Hit counter and viewport storeage
//	GLint hits, viewport[4];
//
//	// Setup selection buffer
//	glSelectBuffer(BUFFER_LENGTH, selectBuff);
//
//	// Get the viewport
//	glGetIntegerv(GL_VIEWPORT, viewport);
//
//	// Switch to projection and save the matrix
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//
//	// Change render mode
//	glRenderMode(GL_SELECT);
//
//	// Establish new clipping volume to be unit cube around
//	// mouse cursor point (xPos, yPos) and extending two pixels
//	// in the vertical and horzontal direction
//	glLoadIdentity();
//	gluPickMatrix(xPos, viewport[3] - yPos, 2, 2, viewport);
//
//	// Apply perspective matrix
//	gluPerspective(45.0f, fAspect, 1.0, 425.0);
//
//	// Draw the scene
//	RenderScene(GL_SELECT);
//
//	// Collect the hits
//	hits = glRenderMode(GL_RENDER);
//
//
//	// If a single hit occured, display the info.
//	if (hits >= 1)
//	{
//		int choose = selectBuff[3];
//		int depth = selectBuff[1];
//
//		for (int i = 0; i < hits; i++)
//		{
//			if (selectBuff[i * 4 + 1] < (GLuint)depth) //获取深度最小的物体(selectBuff是按照ID从小到大排列的)
//			{
//				choose = selectBuff[i * 4 + 3];
//				depth = selectBuff[i * 4 + 1];
//			}
//		}
//		ProcessPlanet(choose);
//	}
//
//	// Restore the projection matrix
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//
//	// Go back to modelview for normal rendering
//	glMatrixMode(GL_MODELVIEW);
//}
//
//void init(void)
//{
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_FLAT);
//	glDepthRange(0.0, 1.0);  /* The default z mapping */
//}
//
//// Process the mouse click
//void MouseCallback(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//		ProcessSelection(x, y);
//}
//
//void ChangeSize(int w, int h)
//{
//	// Prevent a divide by zero
//	if (h == 0)
//		h = 1;
//
//	// Set Viewport to window dimensions
//	glViewport(0, 0, w, h);
//
//	// Calculate aspect ratio of the window
//	fAspect = (GLfloat)w / (GLfloat)h;
//
//	// Set the perspective coordinate system
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	// Field of view of 45 degrees, near and far planes 1.0 and 425
//	gluPerspective(45.0f, fAspect, 1.0, 425.0);
//
//	// Modelview matrix reset
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//	glutInitWindowSize(600, 300);
//	init();
//	glutCreateWindow("Pick a Planet");
//	glutReshapeFunc(ChangeSize);
//	glutMouseFunc(MouseCallback);
//	glutDisplayFunc(Display);
//
//	glutMainLoop();
//
//	return 0;
//}