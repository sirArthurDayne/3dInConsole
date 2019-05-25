#define OLC_PGE_APPLICATION
#include"olcPixelGameEngine.h"

struct vector3d{ float x, y, z; };
struct Triangle{ vector3d points[3];};
struct Mesh    { std::vector<Triangle> tris;} meshCube;
struct Matrix4x4 { float m[4][4] = {0}; };



class ThreeDimensionConsole : public olc::PixelGameEngine
{
public:
	ThreeDimensionConsole() {	sAppName = "3DConsole"; }

	bool OnUserCreate() override
	{
		meshCube.tris = {
			//Sur
			{0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f},
			//Norte
			{0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f},
			//Oeste
			{0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f},
			//este
			{1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f},
			//Arriba
			{0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f},
			//abajo
			{0.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f}
		};

		//valores de prueba de cubo
		float zNear = 0.1f;//valor mas cerca de pantalla
		float zFar = 1000.0f;// valor mas lejano de pantalla
		float fieldOfView = 90.0f;//angulo de vision 
		float aspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float xyScaleFactor = 1.0f / tanf(fieldOfView * 0.5f / 180.0f * 3.141592f);//escala de proyecion x,y
		
		
		//Matrix de proyeccion
		matProyection.m[0][0] = aspectRatio * xyScaleFactor; //x
		matProyection.m[1][1] =  xyScaleFactor; //y
		matProyection.m[2][2] =  zFar / (zFar - zNear); //z
		matProyection.m[3][2] =  (-zFar * zNear) / (zFar - zNear); //z offset
		matProyection.m[2][3] =  1.0f; //valor z extra q permite dividir / z
		matProyection.m[3][3] =  0.0f; //valor extra 
			



		return true;
	}

	void MultiplyMatrixVector( vector3d& in, vector3d& out, Matrix4x4& matrix)
	{
		//multiplicacion en 4 dimensiones
		out.x   = in.x * matrix.m[0][0] + in.y * matrix.m[1][0] + in.z * matrix.m[2][0] + matrix.m[3][0];
		out.y   = in.x * matrix.m[0][1] + in.y * matrix.m[1][1] + in.z * matrix.m[2][1] + matrix.m[3][1];
		out.z   = in.x * matrix.m[0][2] + in.y * matrix.m[1][2] + in.z * matrix.m[2][2] + matrix.m[3][2];
		//valor final para completar la matrix 4x4

		float w = in.x * matrix.m[0][3] + in.y * matrix.m[1][3] + in.z * matrix.m[2][3] + matrix.m[3][3];
		
		//permite convertir a 3 dimensiones cartesianas
			if (w != 0.0f)
			{
				out.x /= w;
				out.y /= w;
				out.z /= w;
			}
	}


	bool OnUserUpdate(float deltaTime) override
	{
		FillRect(0 ,0, ScreenWidth(), ScreenHeight(), olc::BLACK);

		//ratio de cambio de velodidad
		alfa += 1.50f * deltaTime;
		
		//rotacion en x,z
		matRotationX.m[0][0] = 1.0f;
		matRotationX.m[1][1] = cosf(alfa * 0.5f);
		matRotationX.m[1][2] = sinf(alfa * 0.5f);
		matRotationX.m[2][1] = -sinf(alfa * 0.5f);
		matRotationX.m[2][2] = cosf(alfa * 0.5f);
		matRotationX.m[3][3] = 1.0f;

		//rotacionz
		matRotationZ.m[0][0] = cosf(alfa);
		matRotationZ.m[0][1] = sinf(alfa);
		matRotationZ.m[1][0] = -sin(alfa);
		matRotationZ.m[1][1] = cosf(alfa);
		matRotationZ.m[2][2] = 1.0f;
		matRotationZ.m[3][1] = 1.0f;


		for (auto& mesh : meshCube.tris)
		{
			Triangle triproyection, triTranslate, triRotatedZ, triRotatedZX;

			//rotacion desde el origen
			//rotacion z
			for (int i = 0; i < 3; i++)
			{
				MultiplyMatrixVector(mesh.points[i], triRotatedZ.points[i], matRotationZ);
			}
			//rotacion x
			for (int i = 0; i < 3; i++)
			{
				MultiplyMatrixVector(triRotatedZ.points[i], triRotatedZX.points[i], matRotationX);
			}


			//convierte los valores de la proyeccion en z
			triTranslate = triRotatedZX;
			
			for (int k = 0; k < 3; k++)
			{
				triTranslate.points[k].z = triRotatedZX.points[k].z + 3.0f;
			}

			//realizar calculos de proyeccion
			for (int i = 0; i < 3; i++)
			{
				MultiplyMatrixVector(triTranslate.points[i], triproyection.points[i], matProyection);
			}

			//Escalar a pantalla
			for (int j = 0; j < 3; j++)
			{
				//le sumamos 1 para duplicar escala
				triproyection.points[j].x += 1.20f;
				triproyection.points[j].y += 1.20f;
				//adaptar a escala a pantalla
				triproyection.points[j].x *= 0.6f * (float) ScreenWidth();
				triproyection.points[j].y *= 0.6f * (float) ScreenHeight();
			}

		
			//dibujar en pantalla
			DrawTriangle(triproyection.points[0].x, triproyection.points[0].y,//PLEFT 
						 triproyection.points[1].x, triproyection.points[1].y,//PTOP
						 triproyection.points[2].x, triproyection.points[2].y,//PRIGHT
						 olc::Pixel( triproyection.points[0].x /ScreenWidth() * 255 + 40, triproyection.points[0].y / ScreenHeight() * 255 + 35, triproyection.points[0].z /10 *255  - 10));//color
		}

		return true;
	}
private:

	Triangle tri;
	Matrix4x4 matProyection, matRotationX, matRotationZ;
	float alfa = 0;//angulo acumulador
	
};




int main()
{
	ThreeDimensionConsole app;
	
	if (app.Construct(720, 512, 2, 2))
		app.Start();
	else std::cout << "Windows Creation Error" << std::endl;
	return 0;
}