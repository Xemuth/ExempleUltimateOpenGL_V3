#include <Core/Core.h>
#include <UltimateOpenGL_V3/UltimateOpenGL.h>
#include <GLFW/glfw3.h>
using namespace Upp;


const float screenWidth = 800.0f;
const float screenHeight = 600.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
String TransformFilePath(String FilePath);

bool firstMouse=false;
float lastX = screenWidth/2, lastY = screenHeight/2;


UltimateOpenGL_Context context; //Context carrying all scene and object (Basicly context is a game)

CONSOLE_APP_MAIN
{
	
	StdLogSetup(LOG_COUT|LOG_FILE);
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti alliasing 
    context.SetScreenSize(screenHeight,screenWidth);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight,"Hello UltimateOpenGL V2", NULL, NULL);
    
    if (window == nullptr)
    {
        Cout() << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        Exit(-1);
    }
    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        Cout()  << "Failed to initialize GLAD" << "\n";;
        Exit(-1);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);   // Anti alliasing 
    
    Scene& myScene = context.AddScene("main");
    myScene.AddCamera("main");
    myScene.SetBackGroundColor(context.TransformRGBToFloatColor(40,180,200));

	Object3D& modele =  myScene.CreateGameObject<Object3D>("modele");
	modele.LoadModel("C:\\Upp\\myapps\\AssimpTest\\ressource\\nanosuit.obj");

	modele.SetOnDrawFunction([](GameObject& modele){
		double yTransform = glm::cos(context.GetEllapsedTime());
		modele.GetTransform().MoveFrom(glm::vec3(context.GetDeltaTime() * yTransform , 0.0f, 0.0f));
		modele.GetTransform().RotateFromEulerAngles(context.GetDeltaTime() * 1,glm::vec3(1,1,1));
	});

	myScene.Load();
	
    while(!glfwWindowShouldClose(window)) {
        
        processInput(window); //analyses les touches appuyées
        
		glfwSetWindowTitle(window, "UltimateOpenGL V3 - " +AsString(context.GetFPS()) +" FPS");
		try{
			context.Draw(); //déssine la scene et les objets actuellement chargé
		}catch(UGLException& e){
			LOG(e.what());	
		}
	
	    glfwSwapBuffers(window);
	    glfwPollEvents(); 
	}
	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	try{
		Camera& myCamera = context.GetActiveScene().GetActiveCamera();
	    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	        glfwSetWindowShouldClose(window, true);	   
		
		if(myCamera.IsObjectBinded() && myCamera.IsKeyBindingActivate()) {
			/*for(Object3D* go : goKeyboard){
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			        go->KeyboardInput(myCamera, FORWARD, deltaTime);
			    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			        go->KeyboardInput(myCamera, BACKWARD, deltaTime);
			    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			        go->KeyboardInput(myCamera, LEFT, deltaTime);
			    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			        go->KeyboardInput(myCamera, RIGHT, deltaTime);
			}*/
	    } else {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		        myCamera.ProcessKeyboard(FORWARD, context.GetDeltaTime());
		    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		        myCamera.ProcessKeyboard(BACKWARD, context.GetDeltaTime());
		    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		        myCamera.ProcessKeyboard(LEFT, context.GetDeltaTime());
		    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				myCamera.ProcessKeyboard(RIGHT, context.GetDeltaTime());
	    }
	}catch(const UGLException& exception){
		LOG("Exception occured in processInput !\n" + String(exception.what()));
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
    	const Upp::Vector<Camera>& cameras=  context.GetActiveScene().GetAllCamera();
   		int i = 0;
   		for(Camera&  cam : cameras){
   			if(	&cam ==  &context.GetActiveScene().GetActiveCamera()){
   				break;	
   			}
   			i++;
   		}
   		if(myCameras.GetCount() > ++i){
   			context.GetActiveScene().UseCamera(myCameras[i]);
   		}else{
   			context.GetActiveScene().UseCamera(myCameras[0]);
   		}
    }*/
    if (key == GLFW_KEY_N && action == GLFW_PRESS){
        /*
        const Vector<String>& scenesNames = context.GetAllScene()();
		for(const String& name : scenesNames){
			if( &context.GetScene(name)	!= &context.GetActiveScene()){
				context.SetActiveScene(name);
				break;	
			}
		}*/
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS){
       Cout() << "Delta Time:" << AsString( context.GetDeltaTime()) << "   Time Ellapsed :" << AsString(context.GetEllapsedTime()) << EOL;
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	try{
		Camera& myCamera = context.GetActiveScene().GetActiveCamera();
	    if (firstMouse)
	    {
	        lastX = xpos;
	        lastY = ypos;
	        firstMouse = false;
	    }
	
	    float xoffset = xpos - lastX;
	    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	
	    lastX = xpos;
	    lastY = ypos;
	
	    if(myCamera.IsObjectBinded() && myCamera.IsMouseBindingActivate()){
		   /* for(Object3D* go : goMouse){
				go->MouseMove(myCamera, xoffset, yoffset);
		    }*/
	    }else{
	        myCamera.ProcessMouseMovement(xoffset, yoffset);   
	    }
	}catch(const UGLException& exception){
		LOG("Exception occured in mouseCallback !\n"+ String(exception.what()));
	}
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	try{
		Camera& myCamera = context.GetActiveScene().GetActiveCamera();
	    myCamera.ProcessMouseScroll(yoffset);
    }catch(const UGLException& exception){
		LOG("Exception occured in scrollCallback !\n"+ String(exception.what()));
	}
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	context.SetScreenSize(height,width);
    glViewport(0, 0, width, height);
}

String TransformFilePath(String FilePath){
	String FilePathBuffer = String(__FILE__);
	String FilePathTempory ="";
	FilePathBuffer = Replace(FilePathBuffer,Vector<String>{"\\"},Vector<String>{"/"});
	
	Replace(FilePath,Vector<String>{"\\"},Vector<String>{"/"});
	FilePathTempory  = FilePathBuffer.Left(FilePathBuffer.ReverseFind("/")) + FilePath;
	if(!FileExists(FilePathTempory)){
		return (GetExeFolder()  +FilePath);	
	}
	return FilePathTempory;
}