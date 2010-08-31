/**
 * @author Junyun
 */
package visualizer;

import java.lang.Exception;

import javafx.stage.Stage;
import javafx.animation.*;
import javafx.scene.*;
import javafx.scene.image.*;
import javafx.scene.image.Image.*;
import javafx.scene.shape.*;
import javafx.scene.paint.*;
import javafx.scene.input.*;
import javafx.scene.control.*;
import javafx.scene.text.*;
import javafx.scene.transform.*;
import javafx.geometry.*;

import config.VisualizerConfig;
import shape.Screen;

def visualizerConfig                 = VisualizerConfig.getInstance();
def screenWidth:Integer              = visualizerConfig.getInt("visualizer/screenWidth", 986);
def screenHeight:Integer             = visualizerConfig.getInt("visualizer/sceneHeight", 486);
def fieldScreenWidth:Integer         = visualizerConfig.getInt("visualizer/fieldScreenWidth", 686);
def fieldScreenHeight:Integer        = visualizerConfig.getInt("visualizer/fieldScreenHeight", 486);
def imageWidth:Integer               = visualizerConfig.getInt("visualizer/imageWidth", 320);
def imageHeight:Integer              = visualizerConfig.getInt("visualizer/imageHeight", 240);
def horizontalImageSep:Integer       = visualizerConfig.getInt("visualizer/horizontalImageSep", 6);
def fieldLength:Integer              = visualizerConfig.getInt("field/fieldLength", 600);
def fieldWidth:Integer               = visualizerConfig.getInt("field/fieldWidth", 400);
def fieldLineWidth:Integer           = visualizerConfig.getInt("visualizer/fieldLineWidth", 5); 
def penaltyBoxLength:Integer         = visualizerConfig.getInt("field/penaltyBoxLength", 60);
def penaltyBoxWidth:Integer          = visualizerConfig.getInt("field/penaltyBoxWidth", 220);
def centerCircleRadius:Integer       = visualizerConfig.getInt("field/centerCircleRadius", 60);
def penaltyPointLength:Integer       = visualizerConfig.getInt("visualizer/penaltyPointLength", 10); 
def distCenterToPenaltyPoint:Integer = visualizerConfig.getInt("field/distCenterToPenaltyPoint", 120);
def ballColor:String                 = visualizerConfig.getString("visualizer/ballColor","0xFF0000");
def blueGoalPostColor:String         = visualizerConfig.getString("visualizer/blueGoalPostColor","0x0000FF");
def yellowGoalPostColor:String       = visualizerConfig.getString("visualizer/yellowGoalPostColor","0xFFFF00");  
def goalWidth:Integer                = visualizerConfig.getInt("field/distCenterToPenaltyPoint", 140); 
def goalPostLength:Integer           = visualizerConfig.getInt("visualizer/goalPostLength", 20); 
def goalPostWidth:Integer            = visualizerConfig.getInt("visualizer/goalPostWidth", 5); 
def sslVisionAddress:String          = visualizerConfig.getString("sslVision/address", "224.5.23.2");
def sslVisionPort:Integer            = visualizerConfig.getInt("sslVision/port", 10002);

var visualizerStage:Stage;
var currentScene:Scene;
var scene1:Scene;
var scene2:Scene;
//Field Definitions
def staticField1:Group = createFieldGroup();
def staticField2:Group = createFieldGroup();
var fieldShapes1:Group = createFieldShapesGroup();
var fieldShapes2:Group = createFieldShapesGroup();
//Original Image Defintions
var originalImage1:Group = createOriginalImageGroup();
var originalImage2:Group = createOriginalImageGroup();
var originalImageShapes1:Group = createOriginalImageGroup();
var originalImageShapes2:Group = createOriginalImageGroup();
//Segmented Image Defintions
var segmentedImage1:Group = createSegmentedImageGroup();
var segmentedImage2:Group = createSegmentedImageGroup();
var segmentedImageShapes1:Group = createSegmentedImageGroup();
var segmentedImageShapes2:Group = createSegmentedImageGroup();

public class FXInterface extends JavaInterface {

  public override function drawCircle(screenID: Integer,
                                      x: Number, y: Number, r: Number,
                                      fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer,
                                      strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                      strokeThickness: Number) {
    def group = getGroupToDraw(screenID);
    insert Circle {
             centerX: x, centerY: y
             radius: r
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }
    
  public override function drawCircle(screenID: Integer,
                                      x: Number, y: Number, r: Number,
                                      fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer) {
    def group = getGroupToDraw(screenID);
    insert Circle {
             centerX: x, centerY: y
             radius: r
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.TRANSPARENT
             strokeWidth: 0
           } into group.content;
  }

  public override function drawCircle(screenID: Integer,
                                      x: Number, y: Number, r: Number,
                                      strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                      strokeThickness: Number) {
    def group = getGroupToDraw(screenID);
    insert Circle {
             centerX: x, centerY: y
             radius: r
             fill: Color.TRANSPARENT
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }

  public override function drawRectangle(screenID: Integer,
                                         topLeftX: Number, topLeftY: Number,
                                         length: Number, breadth: Number,
                                         fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer) {
    def group = getGroupToDraw(screenID);
    insert Rectangle {
             x: topLeftX, y: topLeftY
             width: length, height: breadth
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.TRANSPARENT
             strokeWidth: 0
           } into group.content;
  }

  public override function drawRectangle(screenID: Integer,
                                         topLeftX: Number, topLeftY: Number,
                                         length: Number, breadth: Number,
                                         strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                         strokeThickness: Number) {
    def group = getGroupToDraw(screenID);
    insert Rectangle {
             x: topLeftX, y: topLeftY
             width: length, height: breadth
             fill: Color.TRANSPARENT
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }

  public override function drawRectangle(screenID: Integer,
                                         topLeftX: Number, topLeftY: Number,
                                         length: Number, breadth: Number,
                                         fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer,
                                         strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                         strokeThickness: Number) {
      def group = getGroupToDraw(screenID);
      insert Rectangle {
               x: topLeftX, y: topLeftY
               width: length, height: breadth
               fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
               stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
               strokeWidth: strokeThickness
             } into group.content;
    }

  public override function drawTriangle(screenID: Integer,
                                        x1: Number, y1: Number,
                                        x2: Number, y2: Number,
                                        x3: Number, y3: Number,
                                        fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer) {
    def group = getGroupToDraw(screenID);
    def pointsTriangle : Number[] = [ [x1, y1], [x2, y2], [x3, y3] ];
    insert Polygon {
             points: pointsTriangle
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.TRANSPARENT
             strokeWidth: 0
           } into group.content;
  }

  public override function drawTriangle(screenID: Integer,
                                        x1: Number, y1: Number,
                                        x2: Number, y2: Number,
                                        x3: Number, y3: Number,
                                        strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                        strokeThickness: Number) {
    def group = getGroupToDraw(screenID);
    def pointsTriangle : Number[] = [ [x1, y1], [x2, y2], [x3, y3] ];
    insert Polygon {
             points: pointsTriangle
             fill: Color.TRANSPARENT
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }
                                    
  public override function drawTriangle(screenID: Integer,
                                        x1: Number, y1: Number,
                                        x2: Number, y2: Number,
                                        x3: Number, y3: Number,
                                        fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer,
                                        strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                        strokeThickness: Number) {
    def group = getGroupToDraw(screenID);
    def pointsTriangle : Number[] = [ [x1, y1], [x2, y2], [x3, y3] ];
    insert Polygon {
             points: pointsTriangle
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }
            
  public override function drawLine(screenID: Integer,
                                    x1: Number, y1: Number,
                                    x2: Number, y2: Number,
                                    strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                    strokeThickness: Number) {
    def group = getGroupToDraw(screenID);
    insert Line {
             startX: x1, startY: y1,
             endX:   x2, endY:   y2,
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }
  
  public override function drawFieldObject(screenID: Integer,
                                           x: Number, y: Number, r: Number,
                                           fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer,
                                           strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                           strokeThickness: Number,
                                           label: String) {
    def group = getGroupToDraw(screenID);
    insert Circle {
             centerX: x, centerY: y
             radius: r
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }
      
  public override function drawFieldObject(screenID: Integer,
                                           x: Number, y: Number, r: Number,
                                           fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer,
                                           strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                           strokeThickness: Number,
                                           positionAlpha: Number, positionR: Integer, positionG: Integer, positionB: Integer,
                                           positionDeviation: Number, 
                                           label: String) {
    def group = getGroupToDraw(screenID);
    insert Circle {
		     centerX: x, centerY: y
		     radius: r+positionDeviation
		     fill: Color.rgb(positionR, positionG, positionB, positionAlpha)
           } into group.content;
    insert Circle {
             centerX: x, centerY: y
             radius: r
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
  }

  public override function drawFieldObject(screenID: Integer,
                                           x: Number, y: Number, r: Number,
                                           fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer,
                                           strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                           strokeThickness: Number,
                                           angleAlpha: Number, angleR: Integer, angleG: Integer, angleB: Integer,
                                           angle: Number, angleDeviation: Number, 
                                           label: String) {
    def group = getGroupToDraw(screenID);  
    insert Circle {
             centerX: x, centerY: y
             radius: r
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
    insert Arc {
             centerX: x, centerY: y
             radiusX: r, radiusY: r
             startAngle: angle - angleDeviation / 2, length: angleDeviation,
             fill: Color.rgb(angleR, angleG, angleB, angleAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
             type: ArcType.ROUND
           } into group.content;
   insert Line {
             startX: x, startY: y
             endX: (x + r * javafx.util.Math.cos(javafx.util.Math.toRadians(angle)))   
             endY: (y - r * javafx.util.Math.sin(javafx.util.Math.toRadians(angle))) 
          } into group.content;   
  }

  public override function drawFieldObject(screenID: Integer,
                                           x: Number, y: Number, r: Number,
                                           fillAlpha: Number, fillR: Integer, fillG: Integer, fillB: Integer,
                                           strokeAlpha: Number, strokeR: Integer, strokeG: Integer, strokeB: Integer,
                                           strokeThickness: Number,
                                           positionAlpha: Number, positionR: Integer, positionG: Integer, positionB: Integer,
                                           positionDeviation: Number, 
                                           angleAlpha: Number, angleR: Integer, angleG: Integer, angleB: Integer,
                                           angle: Number, angleDeviation: Number, 
                                           label: String) {
    def group = getGroupToDraw(screenID);
  
    insert Circle {
             centerX: x, centerY: y
             radius: r+positionDeviation
             fill: Color.rgb(positionR, positionG, positionB, positionAlpha)
           } into group.content;
    insert Circle {
             centerX: x, centerY: y
             radius: r
             fill: Color.rgb(fillR, fillG, fillB, fillAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
           } into group.content;
    insert Arc {
             centerX: x, centerY: y
             radiusX: r, radiusY: r
             startAngle: angle - angleDeviation / 2, length: angleDeviation,
             fill: Color.rgb(angleR, angleG, angleB, angleAlpha)
             stroke: Color.rgb(strokeR, strokeG, strokeB, strokeAlpha)
             strokeWidth: strokeThickness
             type: ArcType.ROUND
           } into group.content;   
    insert Line {
             startX: x, startY: y
             endX: (x + r * javafx.util.Math.cos(javafx.util.Math.toRadians(angle)))   
             endY: (y - r * javafx.util.Math.sin(javafx.util.Math.toRadians(angle))) 
           } into group.content;   
  }
                                      
  public override function drawImage(screenID: Integer,
                                     fxImage: Image) {
    if (screenID == Screen.originalImage.getID()) {
        delete originalImage1.content;
        insert ImageView {
            x: 0, y: 0
            image: fxImage
        } into originalImage1.content;
        
        delete originalImage2.content;
        insert ImageView {
                x: 0, y: 0
                image: fxImage
        } into originalImage2.content;
    }
    else if (screenID == Screen.segmentedImage.getID()) {
          delete segmentedImage1.content;
          insert ImageView {
              x: 0, y: 0
              image: fxImage
          } into segmentedImage1.content;
          
          delete segmentedImage2.content;
          insert ImageView {
                  x: 0, y: 0
                  image: fxImage
          } into segmentedImage2.content;
      }
    }
  
}

function calcScale(mw:Number, mh:Number, iw:Number, ih:Number):Number[]{
    return [(mw/iw), (mh/ih)];
         
}
 
function createFieldGroup():Group {
	return Group{
	    clip: Rectangle{
	        width: fieldScreenWidth, height: fieldScreenHeight
	    }  
	    def realWidth:Number=screenWidth;
	    def realHeight:Number=screenHeight;
	    def scale:Number[]=bind calcScale(visualizerStage.scene.width,visualizerStage.scene.height,realWidth, realHeight);
	    def xoff:Number = bind (visualizerStage.scene.width-scale[0]*realWidth)/2;
	    def yoff:Number = bind (visualizerStage.scene.height-scale[1]*realHeight)/2;
        transforms:bind[Translate.translate(xoff,yoff), Scale.scale(scale[0],scale[1])];
	    content: [	 
	        Rectangle{ //Field
	            x:0, y:0
	            fill:Color.GREEN
	            width: fieldScreenWidth, height: fieldScreenHeight
	        }   
	        Rectangle{ //Field Outline
	          x: (fieldScreenWidth-fieldLength)/2, y: (fieldScreenHeight-fieldWidth)/2
	          width: fieldLength, height: fieldWidth
	          fill: Color.TRANSPARENT
	          stroke: Color.WHITE
	          strokeWidth: fieldLineWidth 
	        }
	        Rectangle{ //Left Penalty Box
	          x: (fieldScreenWidth-fieldLength)/2, y: (fieldScreenHeight-penaltyBoxWidth)/2 
	          width: penaltyBoxLength, height: penaltyBoxWidth
	          fill:Color.TRANSPARENT
	          stroke: Color.WHITE
	          strokeWidth: fieldLineWidth    
	        }
	        Rectangle{ //Right Penalty Box
	          x: (fieldScreenWidth-fieldLength)/2+fieldLength-penaltyBoxLength, y: (fieldScreenHeight-penaltyBoxWidth)/2 
	          width: penaltyBoxLength, height: penaltyBoxWidth
	          fill:Color.TRANSPARENT
	          stroke: Color.WHITE
	          strokeWidth: fieldLineWidth            
	        }
	        Line{ //Center Line
	            startX: fieldScreenWidth/2 startY: (fieldScreenHeight-fieldWidth)/2 
	            endX: fieldScreenWidth/2 endY: (fieldScreenHeight-fieldWidth)/2 + fieldWidth
	            stroke: Color.WHITE
	            strokeWidth: fieldLineWidth
	        }
	        Circle{ //Center Circle
	            centerX: fieldScreenWidth/2, centerY: fieldScreenHeight/2
	            fill: Color.TRANSPARENT
	            radius: centerCircleRadius
	            stroke: Color.WHITE
	            strokeWidth: fieldLineWidth          
	        }
	        Line{ //Center Point
	            startX: fieldScreenWidth/2-(penaltyPointLength/2) startY: fieldScreenHeight/2
	            endX: fieldScreenWidth/2+(penaltyPointLength/2) endY: fieldScreenHeight/2
	            stroke: Color.WHITE
	            strokeWidth: fieldLineWidth
	        } 
	        //Left Penalty Point
	        Line{ //horizontal line
	            startX: fieldScreenWidth/2-distCenterToPenaltyPoint-(penaltyPointLength/2) startY: fieldScreenHeight/2
	            endX: fieldScreenWidth/2-distCenterToPenaltyPoint+(penaltyPointLength/2) endY: fieldScreenHeight/2
	            stroke: Color.WHITE
	            strokeWidth: fieldLineWidth 
	        }
	        Line{ //vertical line
	            startX: fieldScreenWidth/2-distCenterToPenaltyPoint startY: fieldScreenHeight/2-(penaltyPointLength/2)
	            endX: fieldScreenWidth/2-distCenterToPenaltyPoint endY: fieldScreenHeight/2+(penaltyPointLength/2)
	            stroke: Color.WHITE
	            strokeWidth: fieldLineWidth          
	        }   
	        //Right Penalty Point
	        Line{ //horizontal line
	            startX: fieldScreenWidth/2+distCenterToPenaltyPoint-(penaltyPointLength/2) startY: fieldScreenHeight/2
	            endX: fieldScreenWidth/2+distCenterToPenaltyPoint+(penaltyPointLength/2) endY: fieldScreenHeight/2
	            stroke: Color.WHITE
	            strokeWidth: fieldLineWidth           
	        }
	        Line{ //vertical line
	            startX: fieldScreenWidth/2+distCenterToPenaltyPoint startY: fieldScreenHeight/2-(penaltyPointLength/2)
	            endX: fieldScreenWidth/2+distCenterToPenaltyPoint endY: fieldScreenHeight/2+(penaltyPointLength/2)
	            stroke: Color.WHITE
	            strokeWidth: fieldLineWidth        
	        }
	        //Blue Goal Post
	        Rectangle{ //right goal post from the center facing the blue goal post
	            x: (fieldScreenWidth-fieldLength)/2-goalPostLength+(fieldLineWidth/2), y: (fieldScreenHeight-goalWidth)/2
	            width: goalPostLength, height: goalPostWidth
	            fill: Color.web(blueGoalPostColor)
	        }
	        Rectangle{ //left goal post from the center facing the blue goal post
	            x: (fieldScreenWidth-fieldLength)/2-goalPostLength+(fieldLineWidth/2), y: (fieldScreenHeight-goalWidth)/2+goalWidth
	            width: goalPostLength, height: goalPostWidth
	            fill: Color.web(blueGoalPostColor)
	        }
	        Line{ //center goal bar from the center facing the blue goal post
	            startX: (fieldScreenWidth-fieldLength)/2-goalPostLength+(fieldLineWidth/2) startY: (fieldScreenHeight-goalWidth)/2+(goalPostWidth/2)
	            endX: (fieldScreenWidth-fieldLength)/2-goalPostLength+(fieldLineWidth/2) endY: (fieldScreenHeight-goalWidth)/2+goalWidth+(goalPostWidth/2)
	            stroke: Color.web(blueGoalPostColor)
	            strokeWidth: goalPostWidth
	        }        
	        //Yellow Goal Post
	        Rectangle{ //left goal post from the center facing the yellow goal post
	            x: fieldScreenWidth-((fieldScreenWidth-fieldLength)/2), y: (fieldScreenHeight-goalWidth)/2
	            width: goalPostLength, height: goalPostWidth
	            fill: Color.web(yellowGoalPostColor)
	        }
	        Rectangle{ //right goal post from the center facing the yellow goal post
	            x: fieldScreenWidth-((fieldScreenWidth-fieldLength)/2), y: (fieldScreenHeight-goalWidth)/2+goalWidth
	            width: goalPostLength, height: goalPostWidth
	            fill: Color.web(yellowGoalPostColor)
	        }
	        Line{ //center goal bar from the center facing the yellow goal post
	            startX: fieldScreenWidth-((fieldScreenWidth-fieldLength)/2)+goalPostLength startY: (fieldScreenHeight-goalWidth)/2+(goalPostWidth/2)
	            endX: fieldScreenWidth-((fieldScreenWidth-fieldLength)/2)+goalPostLength endY: (fieldScreenHeight-goalWidth)/2+goalWidth+(goalPostWidth/2)
	            stroke: Color.web(yellowGoalPostColor)
	            strokeWidth: goalPostWidth
	        }             
	    ]
	}
}

function createFieldShapesGroup():Group {
    return Group{
        clip: Rectangle{
            width: fieldScreenWidth, height: fieldScreenHeight
        }  
        def realWidth:Number=screenWidth;
        def realHeight:Number=screenHeight;
        def scale:Number[]=bind calcScale(visualizerStage.scene.width,visualizerStage.scene.height,realWidth, realHeight);
        def xoff:Number = bind (visualizerStage.scene.width-scale[0]*realWidth)/2;
        def yoff:Number = bind (visualizerStage.scene.height-scale[1]*realHeight)/2;
        transforms:bind[Translate.translate(xoff,yoff), Scale.scale(scale[0],scale[1])];
        content: [   
           
        ]
    }
}

function createOriginalImageGroup():Group {
    return Group{
        clip: Rectangle{
            width: imageWidth, height: imageHeight
        }  
        def realWidth:Number=screenWidth;
        def realHeight:Number=screenHeight;
        def scale:Number[]=bind calcScale(visualizerStage.scene.width,visualizerStage.scene.height,realWidth, realHeight);
        def xoff:Number = bind ((visualizerStage.scene.width-scale[0]*realWidth)/2 + fieldScreenWidth*scale[0]);
        def yoff:Number = bind (visualizerStage.scene.height-scale[1]*realHeight)/2;
        transforms:bind[Translate.translate(xoff,yoff), Scale.scale(scale[0],scale[1])];
        content: [   
           
        ]
    }
}


        
function createSegmentedImageGroup():Group {
    return Group{
        clip: Rectangle{
            width: imageWidth, height: imageHeight
        }  
        def realWidth:Number=screenWidth;
        def realHeight:Number=screenHeight;
        def scale:Number[]=bind calcScale(visualizerStage.scene.width,visualizerStage.scene.height,realWidth, realHeight);      
        def xoff:Number = bind ((visualizerStage.scene.width-scale[0]*realWidth)/2 + fieldScreenWidth*scale[0]);
        def yoff:Number = bind ((visualizerStage.scene.height-scale[1]*realHeight)/2 + imageHeight*scale[1] + horizontalImageSep*scale[1]);
        transforms:bind[Translate.translate(xoff,yoff), Scale.scale(scale[0],scale[1])];
        content: [   
           
        ]
    }
}

function getGroupToDraw(screenID : Integer) : Group {
  if (screenID==Screen.field.getID()){
      if ((currentScene==null) or (currentScene==scene1)){
          return fieldShapes2;
      } else{
          return fieldShapes1;
      }
  } else if (screenID==Screen.originalImage.getID()){
      if ((currentScene==null) or (currentScene==scene1)){
          return originalImageShapes2;
      } else{
          return originalImageShapes1;
      }
  } else if (screenID==Screen.segmentedImage.getID()){
      if ((currentScene==null) or (currentScene==scene1)){
          return segmentedImageShapes2;
      } else{
          return segmentedImageShapes1;
      }      
  }
  return null;
}

function flipScreen() {
  if ((currentScene==null) or (currentScene==scene1)){
    currentScene = scene2;
  } else{
	currentScene = scene1;
  }
}

function clearPreviousScreen(){
  
  if ((currentScene==null) or (currentScene==scene1)){
    //Field
    delete fieldShapes2.content;
    //Original Image
    //delete originalImage2.content;
    delete originalImageShapes2.content;
    //Segmented Image  
    delete segmentedImage2.content;
    delete segmentedImageShapes2.content; 
  } else{
    //Field
    delete fieldShapes1.content;
    //Original Image
    //delete originalImage1.content;
    delete originalImageShapes1.content;
    //Segmented Image  
    delete segmentedImage1.content;
    delete segmentedImageShapes1.content; 
  }  
}

function printUsage() {
  println("javafx -jar Visualizer.jar [address of robot] [port of robot]");
}

function run(args : String []) {
  //TODO   
  if (args.size() < 1) {
    printUsage();
    return null;
  }
  def robotAddress = args[0];
  var robotPort    = visualizerConfig.getInt("comm/remoteListenPort", 10000);
  if (args.size() >= 2) {
    var port = 0;
    try {
      port = Integer.parseInt(args[1]);
      if (port > 0) {
        robotPort = port;
      }
      else {
        printUsage();
        return null;
      }
    }
    catch (e : Exception) {
      printUsage();
      return null;
    }
  }
  
  var fxInterface = FXInterface{}

  scene1 = Scene{
    width: screenWidth
    height: screenHeight
    fill: Color.BLACK
    content:[
     staticField1,
     fieldShapes1,
     originalImage1,
     originalImageShapes1, 
     segmentedImage1,
     segmentedImageShapes1         
    ]     
  }

  scene2 = Scene{
     width: screenWidth
     height: screenHeight
     fill: Color.BLACK
     content:[
      staticField2,
      fieldShapes2,
      originalImage2,
      originalImageShapes2, 
      segmentedImage2,
      segmentedImageShapes2         
     ]     
   }
    
  VisualizerHandler.getInstance().connectToRobot(robotAddress, robotPort);
  VisualizerHandler.getInstance().connectToSSLVision(sslVisionAddress, sslVisionPort);
    
  currentScene = scene1;
  
  visualizerStage = Stage{
    title: "CMurfs Visualizer"
    scene: bind currentScene	  
  }
  
  Timeline {
    repeatCount: Timeline.INDEFINITE
    keyFrames: KeyFrame {
      time: 70ms
      action: function() {
        clearPreviousScreen();
        def toFlip : Boolean = VisualizerHandler.getInstance().run(fxInterface);
        if (toFlip) {
          flipScreen();
        }
      }
    }
        
  }.play()
}
