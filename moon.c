#include<GL/glut.h>
#include<math.h>
GLfloat xRotated,yRotated,zRotated;
GLdouble radius=1;
char ch=0;
GLfloat qaBlack[]={0.0,0.0,0.0,1.0};
GLfloat qaBlue[]={0.0,0.5,1.0,1.0};
GLfloat qaWhite[]={1.0,1.0,1.0,1.0};
GLfloat qaRed[]={1.0,0.0,0.0,1.0};

//Set lighting intensity and COLOR16

GLfloat qaAmbientLight[]={0.1,0.1,0.1,1.0};
GLfloat qaSunAmbientLight[]={1.0,1.0,0.0,1.0};
GLfloat qaDiffuseLight[]={1,1,1,1.0};
GLfloat qaSpecularLight[]={0.0,0.0,0.0,1.0};
GLfloat emitLight[]={0.9,0.9,0.9,0.01};
GLfloat emitSunLight[]={1.0,0.4,0.0,0.01};
GLfloat Noemit[]={0.0,0.0,0.0,1.0};

//Light source position

GLfloat qaLightPosition[]={0,0,0,1};
GLfloat qaSunLightPosition[]={0,0,0,1};

void display(void);
void reshape(int x,int y);
void doFrame()
{
    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, doFrame, 0);
}

void idleFunc(void)
{
    if(zRotated>360.0)
    {
        zRotated-=360.0*floor(zRotated/360.0);  //Don't allow overflow
    }
    if(yRotated>360.0)
    {
        yRotated-=360.0*floor(yRotated/360.0);  //Don't allow overflow
    }

    zRotated+=0.1;
    yRotated+=0.1;
    display();
}
void sun()
{
   glPushMatrix();
    glTranslatef(15,0.0,1.0);
    glScalef(1.0,1.0,0.0);
    glLightfv(GL_LIGHT1,GL_POSITION,qaSunLightPosition);
    glMaterialfv(GL_FRONT,GL_EMISSION,emitSunLight);  //Make Sphere glow
    glutSolidSphere(radius*5,50,150);
    glMaterialfv(GL_FRONT,GL_EMISSION,Noemit);
    glPopMatrix();
}
void earth()
{
    glPushMatrix();
    glTranslatef(0.0,0.0,0);
    glColor3f(1,1,1);
    //Set material properties
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,qaBlue);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,qaBlue);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,qaWhite);

    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0.2);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glutSolidSphere(radius,50,150);
    glPopMatrix();
}
void moon()
{
    glPushMatrix();
    if(ch==0)
        glRotatef(-yRotated,0.0,1.0,0.0);
    else
        glRotatef(yRotated,0.0,1.0,0.0);
    glTranslatef(2.0,0.0,0.0);

    //Set the light position

    glLightfv(GL_LIGHT0,GL_POSITION,qaLightPosition);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,emitLight);  //Make Sphere glow

    glutSolidSphere(radius/4,50,50);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Noemit);
    glPopMatrix();
}
void orbit()
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslatef(20,0.0,0.0);
    glScalef(1.0,1.0,0.0);
    glutSolidTorus(radius*0.03,radius*20,2,300);
    glPopMatrix();
}
void initLighting()
{
    //Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    //Set lighting intensity and color
    glLightfv(GL_LIGHT0,GL_AMBIENT,qaAmbientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,qaDiffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,qaSpecularLight);

    glLightfv(GL_LIGHT1,GL_AMBIENT,qaSunAmbientLight);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,qaDiffuseLight);
    glLightfv(GL_LIGHT1,GL_SPECULAR,qaSpecularLight);
}

void display(void)
{

    glMatrixMode(GL_MODELVIEW);
    //glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth(0.0);
    glShadeModel(GL_SMOOTH);



    glTranslatef(0.0,0.0,-10.0);

    orbit();

    sun();


    earth();

    moon();



    glutSwapBuffers();
}
void mouse(int button, int state,int x,int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glutTimerFunc(1000.0/60.0, doFrame, 0);
        glutIdleFunc(idleFunc);
        ch=0;
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        glutTimerFunc(1000.0/60.0, doFrame, 0);
        glutIdleFunc(idleFunc);
        ch=1;
    }
}
void reshape(int x,int y)
{
    if(y==0 || x==0) return;
    glMatrixMode(GL_PROJECTION);
    gluPerspective(620.0,(GLdouble)x/(GLdouble)y,0.6,40.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);   //Use the whole window for rendering
}

int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920,1080);
    glutCreateWindow("The Moon Around The Earth");
    initLighting();
    glutMouseFunc(mouse);
    xRotated=yRotated=zRotated=0.0;
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

}

