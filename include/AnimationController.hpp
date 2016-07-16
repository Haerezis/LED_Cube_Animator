#ifndef __ANIMATION_CONTROLLER_HPP
#define __ANIMATION_CONTROLLER_HPP

#include <QObject>
#include <QStandardItemModel>
#include "AnimationFrameController.hpp"
#include "Animation.hpp"
#include "CubeOpenGL.hpp"
#include "ui_MainWindows.h"


class AnimationController : public QObject
{
  Q_OBJECT
  public:
    AnimationController(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi);
    ~AnimationController() {}

    void setupConnect();

    bool reset(unsigned int cubeSize);
    bool load();
    bool save();

    void updateAnimationState(bool modificationNotSaved);

  public slots:
    void addFrame();
    void setSelectedFramesDuration();
    bool newAnimation();
    bool openAnimation();
    bool saveAnimation();
    bool saveAnimationAs();
    bool generateData();
    bool quitApplication();
    void frameSelected();
    void animationUpdated() { updateAnimationState(true); }

  private:
    std::vector<unsigned int> _getSelectedFramesIndex();
    bool _unsavedModificationDialog(const std::string& text, const std::string& informativeText);
    bool _modificationNotSaved;

    QMainWindow& _mainWindow;
    Ui::MainWindow& _mainWindowUi;
    QSpinBox& _duration;
    CubeOpenGL& _cubeOpenGL;

    std::string _filepath;

    Animation _animation;
    QStandardItemModel _frameList;

    static const unsigned int _defaultCubeSize;
};

#endif
