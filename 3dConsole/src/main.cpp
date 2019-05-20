#define OLC_PGE_APPLICATION
#include"olcPixelGameEngine.h"


class ThreeDimensionConsole : public olc::PixelGameEngine
{
public:
	ThreeDimensionConsole() {	sAppName = "3DConsole"; }

	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float deltaTime) override
	{
		return true;
	}
private:
	struct vector3d{ float x, y, z; };
	struct Triangle{ vector3d points[3];};
	struct Mesh    { std::vector<Triangle> tris;};
};




int main()
{
	ThreeDimensionConsole app;
	
	if (app.Construct(512, 480, 2, 2))
		app.Start();
	else std::cout << "Windows Creation Error" << std::endl;
	return 0;
}