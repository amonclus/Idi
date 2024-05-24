// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <vector>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::treat_timeout() {
    makeCurrent();

    angleCar[0] += 2.0/360;
    rotationCar[0] += 2.0;
    angleCar[1]  += 3.0/360;
    rotationCar[1] += 3.0;

    update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
    if(e->x() > x_ant) angleY -= 0.05;
    else if(e->x() < x_ant) angleY += 0.05;

    if(e->y() > y_ant) angleX += 0.05;
    else if(e->y() < y_ant) angleX -= 0.05;

    x_ant = e->x();
    y_ant = e->y();
    viewTransform();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
    angleCar[0] += 2.0/360;
    rotationCar[0] += 2.0;
    angleCar[1]  += 3.0/360;
    rotationCar[1] += 3.0;

    break;
	}
  case Qt::Key_L: {
      if(light) {
          glUniform3f(colFocusLoc, 0.1, 0.1, 0.1);
          glClearColor(0.3, 0.3, 0.3, 1.0);
      }
      else {
          glUniform3f(colFocusLoc, 0.8, 0.8, 0.8);
          glClearColor(0.5, 0.7, 1.0, 1.0);
      }
      light = !light;
    break;
	}
  case Qt::Key_S: {
      if(not toggled) timer.start(30);
      else timer.stop();
      toggled = !toggled;
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();

}


void MyGLWidget::paintGL ()
{
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    // glViewport (0, 0, ample, alt);
    // Esborrem el frame-buffer i el depth-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform3f(colorFondoLoc, 0.0, 0.5, 0.5);
    glUniform3f(especularidadLoc, 0.8, 0.8, 0.8);

    // TERRA
    glBindVertexArray (VAO_Terra);
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniform3f(colorFondoLoc, 1.0, 1.0, 1.0);

    glUniform3f(especularidadLoc, 0.1, 0.1, 0.1);

    // Road
    glm::vec3 pos = glm::vec3(5, 0.01, -5);
    for(float i = 0; i < 4; ++i) {
        glBindVertexArray (VAO_models[ROAD]);
        modelTransformRoad (pos, 90.0 * i);
        glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
    }

    // Car

    for(int i = 0; i < 2; ++i){
        if(i == 0)     glUniform3f(colorFondoLoc, 1.0, 0.0, 0.0);
        else     glUniform3f(colorFondoLoc, 0.0, 1.0, 0.0);

        /*
        posFocus = posCar[i];
        glUniform3fv(posFocusLoc, 1, &posFocus[0]);
        glUniform3f(colFocusLoc, 0.6, 0.6, 0.0);
         */

        glBindVertexArray (VAO_models[CAR]);
        modelTransformCar (posCar[i], angleCar[i], rotationCar[i]);
        glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
    }
    posFocus = glm::vec3(0, 5, 0);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);


    // Pipe
    glBindVertexArray (VAO_models[PIPE]);
    modelTransformPipe ();
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

    glBindVertexArray(0);
}

void MyGLWidget::iniEscena ()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(treat_timeout()));

    colorFondoLoc = glGetUniformLocation (program->programId(), "colorFondo");
    especularidadLoc = glGetUniformLocation (program->programId(), "especularidad");
    posFocusLoc = glGetUniformLocation(program->programId(), "posFocus");
    colFocusLoc = glGetUniformLocation(program->programId(), "colFocus");

    glUniform3f(colFocusLoc, 0.8, 0.8, 0.8);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);

    centreEsc = glm::vec3 (0, 5, 0);
    radiEsc = distance(minEscena, maxEscena)/2;

}

void MyGLWidget::iniCamera ()
{
    angleY = 0.0;
    angleX = glm::radians(45.0);
    projectTransform();
    viewTransform ();
}

void MyGLWidget::viewTransform ()
{
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);
/*
    posFocus = (posFocus * View);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    */

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);


}


void MyGLWidget::modelTransformTerra ()
{
    glm::mat4 TG(1.0f);
    TG = glm::scale(TG, glm::vec3(2.5, 2.5, 2.5));
    TG = glm::translate(TG, glm::vec3(-5, 0, -5));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformRoad (glm::vec3 pos, float angle)
{
    glm::mat4 TG(1.0f);
    TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0, 1, 0));
    TG = glm::translate(TG, pos);
    TG = glm::scale(TG, glm::vec3(5*escalaModels[ROAD]));
    TG = glm::translate(TG, -centreBaseModels[ROAD]);

    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPipe ()
{
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0,0,0));
    TG = glm::scale(TG, glm::vec3(5*escalaModels[PIPE]));
    TG = glm::translate(TG, -centreBaseModels[PIPE]);

    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCar (glm::vec3 radi, float angle, float rotation)
{
    glm::mat4 TG(1.0f);
    TG = glm::rotate(TG, glm::radians(rotation), glm::vec3(0, 1, 0));
    TG = glm::translate(TG, radi);
    TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
    //rotamos para que mire a la dirección que queramos
    TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
    TG = glm::translate(TG, -centreBaseModels[CAR]);

    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}







