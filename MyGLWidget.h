// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>
#include  <iostream>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
    void paintGL();
    void iniEscena();
    void iniCamera();
    void modelTransformTerra();
    void modelTransformRoad (glm::vec3 pos, float angle);
    void modelTransformPipe();
    void modelTransformCar(glm::vec3 radi, float angle, float rotation);
    void viewTransform();

public slots:
    void treat_timeout();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);

  private:
    QTimer timer;
    bool toggled = false;

    GLuint colorFondoLoc, especularidadLoc, posFocusLoc, colFocusLoc;

    float angleX;
    int printOglError(const char file[], int line, const char func[]);
    glm::vec3 maxEscena = glm::vec3(12.5, 15, 12.5);
    glm::vec3 minEscena = glm::vec3(-12.5, 0, -12.5);
    glm::vec3 centreEscena, obs, vrp, up;
    float radiEscena;
    float l, r, b, t, ZN, ZF;
    float x_ant, y_ant;
    glm::vec3 posCar[2] = {glm::vec3(6.5, 0, 0), glm::vec3(9, 0, 0)};
    float angleCar[2] = {0.0, 0.0};

    float rotationCar[2] = {0.0, 0.0};

    glm::vec3 posFocus = glm::vec3(0, 5, 0);
    bool light = true;


};
