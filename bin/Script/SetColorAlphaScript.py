from PythonQt import QtCore, QtGui

print('Show Set Color Script')

# 创建一个窗口
mainWidget = QtGui.QWidget()
mainWidget.setWindowTitle("Color Alpha Set");
mainLayout = QtGui.QVBoxLayout(mainWidget);

# 创建ComboBox
topWidget = QtGui.QWidget()
mainLayout.addWidget(topWidget)
topLayout = QtGui.QHBoxLayout(topWidget)
topLabel = QtGui.QLabel("Attribute Name: ")
topLayout.addWidget(topLabel)
global attrComboBox
attrComboBox = QtGui.QComboBox()
attrComboBox.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
topLayout.addWidget(attrComboBox)

# 创建Slider
bottomWidget = QtGui.QWidget()
mainLayout.addWidget(bottomWidget)
bottomLayout = QtGui.QHBoxLayout(bottomWidget)
global bottomLabel
bottomLabel = QtGui.QLabel("Alpha(100%): ")
bottomLayout.addWidget(bottomLabel)
global alphaSlider
alphaSlider = QtGui.QSlider(QtCore.Qt.Horizontal)
alphaSlider.setMinimum(0)
alphaSlider.setMaximum(255)
bottomLayout.addWidget(alphaSlider)

# 按钮
buttonWidget = QtGui.QWidget()
mainLayout.addWidget(buttonWidget)
buttonLayout = QtGui.QHBoxLayout(buttonWidget)
pLoadButton = QtGui.QPushButton("Load")
pLoadButton.setFixedWidth(100)
pOKButton = QtGui.QPushButton("OK")
pOKButton.setFixedWidth(100)
buttonLayout.addStretch()
buttonLayout.addWidget(pLoadButton)
buttonLayout.addWidget(pOKButton)

def onClickedLoadButton():
    nodeNames = easyCanvas.getSelectedNodes()
    attributeNames = easyCanvas.getAttributeNames(nodeNames[0])
   
    global attrComboBox
    attrComboBox.clear()
    
    for attrName in attributeNames:
        fullAttrName = nodeNames[0] + '.' + attrName
        attrTypeMame = easyCanvas.getAttributeType(fullAttrName)
        if attrTypeMame == 'Color':
            attrComboBox.addItem(fullAttrName)
        
def onMClickedOKButton():
    global attrComboBox
    attrName = attrComboBox.currentText
    color = easyCanvas.getValue(attrName)
    
    global alphaSlider
    alpha = alphaSlider.value
    color.setAlpha(alpha)
    
    easyCanvas.setAttribute(attrName, color)
    
def onSliderChanged(value):
    text = "Alpha(" + str(value) + ")"
    bottomLabel.setText(text)
    
def onComboBoxIndexChanged(str):
    global attrComboBox
    attrName = attrComboBox.currentText
    color = easyCanvas.getValue(attrName)
    global alphaSlider
    alphaSlider.setValue(color.alpha())
            
# 连接信号和槽函数
pLoadButton.connect('clicked()', onClickedLoadButton)
pOKButton.connect('clicked()', onMClickedOKButton)
alphaSlider.connect('valueChanged(int)', onSliderChanged)
attrComboBox.connect('currentIndexChanged(QString)', onComboBoxIndexChanged)
    
mainWidget.resize(480, 180);
mainWidget.show()
