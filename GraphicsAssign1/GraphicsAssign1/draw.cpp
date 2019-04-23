#include <cmath>
#include "Draw.h"
#include<algorithm>
#include "objloader.h"
#define PI 3.1415926535

#define BVIEW_HALF_W 400
#define BVIEW_HALF_H 225

using namespace std;

map<string, GLuint> VAO_map;

void myReshape(int width, int height)
{	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	ReshapeFlag = !ReshapeFlag;
	//나머지 부분을 아래쪽으로: 공을 추적하기 위해 공이 움직일 때마다 카메라 움직여야 함
	
	//lookAtByCamMode(camMode);
}

void display()
{
	if (ReshapeFlag) {
		GameManager::getInstance().FreshTime();
		ReshapeFlag = !ReshapeFlag;
	}

	if (GameManager::getInstance().WhoFinallyWin != 0) {

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 1.0f, 1.0f);
		representResult();
		glLoadIdentity();
		glutSwapBuffers();
		return;
	}
		

	//glClearColor(224/255.0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	vector<GameObjectTree>::iterator iter; 
	
	for (iter = objectsTreeVectorForDraw.begin(); iter != objectsTreeVectorForDraw.end(); iter++)
	{
		
		drawObjectRecursive(iter->root);
	}
	


	/*
	representBox(GameManager::getInstance().screen, 255, 255, 255);
	representBox(GameManager::getInstance().enemyBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().playerBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().net, 204, 204, 204); //grey
	representCircle(GameManager::getInstance().ball);
	*/
	glLoadIdentity();
	representScore(GameManager::getInstance().myScore, 100.0, 800.0);
	glLoadIdentity();
	representScore(GameManager::getInstance().enemyScore, 1400.0, 800.0);
	

	glutSwapBuffers();

}

void drawObjectRecursive(GameObjectNode* root)
{
	glPushMatrix();
	//위치이동
	glTranslated(root->data->object->position.x, root->data->object->position.y, 0.0);

	//회전축으로 이동한 후 회젼, 되돌리기
	glTranslated(root->data->object->rotationAxis.x, root->data->object->rotationAxis.y, 0.0);
	glRotated(root->data->object->rotation, 0., 0., 1.);
	glTranslated(- root->data->object->rotationAxis.x, - root->data->object->rotationAxis.y, 0.0);

	representComponent(*(root->data->object));

//	GameObjectNode *tmp;
	if (root->successor != NULL) drawObjectRecursive(root->successor);
	glPopMatrix();

	if (root->sibling != NULL) drawObjectRecursive(root->sibling);
	
	
	
}

void representComponent(const Transform &object)
{
	if (object.shape == Object::BOX)
		representWall(object);
	else
		representPolygon(object);
		
}



void representPolygon(const Transform &object)
{
	ObjData* drawingObjData;
	
	//load image
	drawingObjData = GetObj(object.shape );

	//shader drawing
	float* scaledVertexArray = (float*)malloc(sizeof(float) * drawingObjData->vertexSize);

	for (int i = 0; i < drawingObjData->vertexSize; i++) {
		switch (i % 3) {
		case 0: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object.GetSize().x + object.position.x;
		case 1: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object.GetSize().y + object.position.y;
		case 2: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object.GetSize().z + object.position.z;
		}
		
		assert(1 && "not implemented yet: scaling and moving");
	}

	// starts to use

	// make buffers
	GLuint VBO, VAO, EBO; //each vertex buffer, vertex array, Elemental buffer

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(drawingObjData->vertices), drawingObjData->vertexArray, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawingObjData->triangleArray), drawingObjData->triangleArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//copy end
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(drawingObjData->triangleArray) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}

void representWall(const Transform &object)
{
	for (int i = 0; i < 5; i++) {
		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		vector<float> vertices;
		vector<unsigned int> indices;

		for (int j = 0; j < 10; i++)
			for (int k = 0; k < 10; k++) {
				vertices.push_back()
			}
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
}


ObjData* GetObj(Object::Shape shape) {
	static ObjData* objData[5]; // size of objcet shape flags
	if (objData[shape] == NULL)
	{
		objData[shape] = new ObjData;
		loadOBJ("path", objData[shape]);
	}
	return objData[shape];
}


void SetModelAndViewMatrix(CamMode camMode) { //reference: https://heinleinsgame.tistory.com/12
	Transform* player = GameManager::getInstance().player;
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 up; // 수정 필요
	glm::mat4 view;
	glm::mat4 Projection; // 월드 좌표로 표현 수정 필요
	switch(camMode){
	case CHARACTER: 
		cameraPos = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z);
		cameraTarget = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z + 1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // 월드 좌표로 표현 수정 필요
		break;
	case BEHIND:
		cameraPos = glm::vec3(WORLD_COORD_MAP_XLEN/2, 10, 0);
		cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN / 2, 10, 1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // 월드 좌표로 표현 수정 필요
		break;
	case HANGING:
		float x, y, z;

		cameraPos = glm::vec3(10, WORLD_COORD_MAP_YLEN, 10);
		cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN/2 ,0, WORLD_COORD_MAP_ZLEN/2);
		up = glm::vec3(0.0f, 1.0f, 0.0f); // 수정 필요
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // 월드 좌표로 표현 수정 필요
		break;
	}

	glUniformMatrix4fv(glGetUniformLocation(Putshader, "View"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Putshader, "Projection"), 1, GL_FALSE, &Projection[0][0]);
}


void representScore(int score, GLfloat x, GLfloat y)
{
	glTranslatef(x, y, 0.0f);
	glScalef(0.5f, 0.5f, 1.0f);
	char s[5];
	_itoa_s(score, s, 10);

	if (score / 10 > 0) {
		_itoa_s(score / 10, s, 10);
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&s[0]);
	}

	_itoa_s(score % 10, s, 10);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&s[0]);
	/*
	for (int i = 0; i < strlen(s); i++)
	{
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&s[i]);
	}*/


}

void representResult(void)
{
	glLoadIdentity();

	glTranslatef(100.0f, 600.0f, 0.0f);
	glScalef(1.5f, 1.5f, 1.0f);
	char winMessage[8] = "YOU WIN";
	char loseMessage[9] = "YOU LOSE";

	if (GameManager::getInstance().WhoFinallyWin == 1) 
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&winMessage);
	else if (GameManager::getInstance().WhoFinallyWin == 2) 
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&loseMessage);
		
}


/*
void representBox(const Transform& box)
{
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat w = box.GetSize().x;
	GLfloat h = box.GetSize().y;


	glColor4f(box.myColor.r / 255.0, box.myColor.g / 255.0, box.myColor.b / 255.0, box.myColor.a);
	glBegin(GL_QUADS);

	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
}

void representCircle(const Transform &circle)
{
	int lineNum = 100; // lineNum각형 으로 근사

	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat r_x = circle.GetSize().x / 2;
	GLfloat r_y = circle.GetSize().y / 2;


	glColor4f(circle.myColor.r / 255.0, circle.myColor.g / 255.0, circle.myColor.b / 255.0, circle.myColor.a);
	glBegin(GL_POLYGON);

	for (int i = 0; i < lineNum/2; i++)
	{
		glVertex2f(x + r_x + r_x * sinf(i * PI * 2 / lineNum), y + r_y + r_y * cosf(i * PI * 2 / lineNum));

		float colR = (circle.myColor.r * (lineNum / 2 - i) + 1 * i) / (float)(lineNum / 2);
		float colG = (circle.myColor.g * (lineNum / 2 - i) + 1 * i) / (float)(lineNum / 2);
		float colB = (circle.myColor.b * (lineNum / 2 - i) + 1 * i) / (float)(lineNum / 2);
		glColor4f(colR, colG, colB, 1);
	}
	for (int i = lineNum/2; i < lineNum; i++)
	{
		glVertex2f(x + r_x + r_x * sinf(i * PI * 2 / lineNum), y + r_y + r_y * cosf(i * PI * 2 / lineNum));

		float colR = (circle.myColor.r * (i - lineNum / 2 ) + 1 * (lineNum - i)) / (float)(lineNum / 2);
		float colG = (circle.myColor.g * (i - lineNum / 2) + 1 * (lineNum - i)) / (float)(lineNum / 2);
		float colB = (circle.myColor.b * (i - lineNum / 2) + 1 * (lineNum - i)) / (float)(lineNum / 2);
		glColor4f(colR, colG, colB, 1);
	}



	glEnd();
}

void representTriangle(const Transform & triangle)
{
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat w = triangle.GetSize().x;
	GLfloat h = triangle.GetSize().y;


	glColor4f(triangle.myColor.r / 255.0, triangle.myColor.g / 255.0, triangle.myColor.b / 255.0, triangle.myColor.a);
	glBegin(GL_TRIANGLES);

	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w / 2, y + h);
	glEnd();
}
*/

/*void lookAtBall(const Object& ball) // to be modified
{

	GLdouble lookAtX = ball.GetCurrentPosition().x + ball.GetSize().x / 2;
	GLdouble lookAtY = ball.GetCurrentPosition().y + ball.GetSize().y / 2;
	GLdouble screenX = WORLDCOORDWINDOWWIDTH;
	GLdouble screenY = WORLDCOORDWINDOWHEIGHT;

	if (lookAtX > screenX - BVIEW_HALF_W)
		lookAtX = screenX - BVIEW_HALF_W;
	else if (lookAtX < BVIEW_HALF_W)
		lookAtX = BVIEW_HALF_W;

	if (lookAtY > screenY - BVIEW_HALF_H)
		lookAtY = screenY - BVIEW_HALF_H;
	else if (lookAtY < BVIEW_HALF_H)
		lookAtY = BVIEW_HALF_H;


	gluLookAt(lookAtX, lookAtY, 0.0f,
		lookAtX, lookAtY, -100.0f,
		0.0f, 1.0f, 0.0f);
	printf("%f\n", lookAtX);
}*/
