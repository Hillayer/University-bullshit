#include <windows.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <new>

#define PI 3.14159265

LRESULT CALLBACK DCDemoGraphProc(HWND, UINT, WPARAM, LPARAM);


class Vector {
public:
	float x, y, z;

	Vector operator +(Vector &v);//Сложение векторов
	Vector& operator +=(Vector &v);//Сложение векторов(ez)
	Vector operator -(Vector &v);//Разность векторов
	Vector operator -();//Обращение векторов
	Vector& operator -=(Vector &v);//Разность векторов(ez)
	Vector operator *(Vector &v);//Векторное умножение
	Vector operator ^(float a);//Умножение вектора на число
	float operator ^(Vector &v);//Скалярное умножение(Coming soon)

	Vector() {
		delete &x;
		delete &y;
		delete &z;
	}

	Vector(float a, float b , float c) {
		x = a;
		y = b;
		z = c;
	}

	Vector(const Vector &v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	~Vector() {}

	void SetVector(float x1, float y1, float z1) {
		x = x1;
		y = y1;
		z = z1;
	}

	void Norm() {
		float d = 1/(*this).Mod();
		(*this).VNumMult(d);
	}
	void VNumMult(float a) {
		(*this).SetVector(x * a, y * a, z * a);
	}
	void VVecMult(Vector t) {
		(*this).SetVector(y*t.z - z*t.y, z*t.x - x*t.z, x*t.y - y*t.x);
	}
	float Mod() {
		return sqrt(x*x + y*y + z*z);
	}
	void VSumm(Vector a) {
		(*this).SetVector(x + a.x, y + a.y, z + a.z);
	}
	float ScMod(Vector a) {
		return x * a.x + y * a.y + z * a.z;
	}
	Vector Proj(Vector vec){
		Vector p1 = Vector(vec);
		p1.Norm();
		p1 = p1 ^ (p1 ^ (*this));
		return (*this) - p1;
	}
	Vector Image(float f, Vector vec) {
		float d;
		d = vec.ScMod(*this) / vec.Mod();
		Vector p = Vector(*this);
		p = p ^ (-f / (d - f));
		Vector e1(1, 0, 0);
		Vector e2(0, 1, 0);
		Vector e3 = Vector(vec);
		e3.Norm();
		if (vec.x != 0 || vec.y != 0) {
			e1 = Vector(-e3.y, e3.x, 0);
			e1.Norm();
			e2 = Vector(e3);
			e2.VVecMult(e1);
			e2.Norm();
		}
		float x1 = e1 ^ p;
		float y1 = e2 ^ p;
		return Vector(x1, y1, 0);
	}
};

float Vector::operator ^(Vector &v)
{
	return v.x * x + v.y * y + v.z * z;
}

Vector Vector::operator *(Vector &v)
{
	return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

Vector Vector::operator ^(float a)
{
	return Vector(x * a, y * a, z * a);
}

Vector Vector::operator -(Vector &v) 
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator -() 
{
	return Vector(-x, -y, -z);
}

Vector& Vector::operator +=(Vector &v)
{	
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector& Vector::operator -=(Vector &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector Vector::operator +(Vector &v)
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

void DrawCLine(HDC hDC, Vector *v1, int m, int n, Vector vec, Vector mv, Vector her, float focus)//DrawCurveLine function
{
	for (int i = 1; i < m; i++) {
		Vector a1 = Vector(v1[i - 1]);
		//a1.VSumm(mv);
		a1 += mv;
		a1.Image(focus, vec); 
		a1.VNumMult(n); 
		//a1.VSumm(her);
		a1 += her;
		Vector a2 = Vector(v1[i]);
		//a2.VSumm(mv); 
		a2 += mv;
		a2.Image(focus, vec); 
		a2.VNumMult(n); 
		//a2.VSumm(her);
		a2 += her;
		MoveToEx(hDC, int(a1.x), int(a1.y), NULL);
		LineTo(hDC, int(a2.x), int(a2.y));
		//delete &a1, &a2;
	}
	
}



void DrawTor(HDC hDC,Vector vec, Vector mv, Vector her, Vector beg, float focus, int a, int b, int n)//Tor function
{
	for (float phy = 0; phy < 2*PI; phy += PI/18) {
		for (float psi = 0; psi < 2 * PI;) {
			Vector a1 = Vector((a * cos(psi) + b) * cos(phy), (a * cos(psi) + b) * sin(phy), a * sin(psi));
			psi += PI / 180;
			Vector a2 = Vector((a * cos(psi) + b) * cos(phy), (a * cos(psi) + b) * sin(phy), a * sin(psi));
			a1 = ((a1 + beg + mv).Image(focus, vec) ^ n) + her;
			a2 = ((a2 + beg + mv).Image(focus, vec) ^ n) + her;
			MoveToEx(hDC, int(a1.x), int(a1.y), NULL);
			LineTo(hDC, int(a2.x), int(a2.y));
		}
	}
	for (float psi = 0; psi < 2 * PI; psi += PI / 5) {
		for (float phy = 0; phy < 2 * PI;) {
			Vector a1((a * cos(psi) + b) * cos(phy), (a * cos(psi) + b) * sin(phy), a * sin(psi));
			phy += PI / 180;
			Vector a2((a * cos(psi) + b) * cos(phy), (a * cos(psi) + b) * sin(phy), a * sin(psi));
			a1 = ((a1 + beg + mv).Image(focus, vec) ^ n) + her;
			a2 = ((a2 + beg + mv).Image(focus, vec) ^ n) + her;
			MoveToEx(hDC, int(a1.x), int(a1.y), NULL);
			LineTo(hDC, int(a2.x), int(a2.y));
		}
	}
}

//---------------PARABOLA HUI-----------------
/*
	Vector *v1, *v2;
	v1 = new Vector[m];
	v2 = new Vector[m];

	//Right parabola wing
	for (int i = 0; i < m; i++) {
		v1[i].x = i*dx;
		v1[i].y = i*i*dx*dx;
		v1[i].z = -1;
	}
	//Left parabola wing

	for (int i = 0; i < m; i++) {
		v2[i].x = -i*dx;
		v2[i].y = i*i*dx*dx;
		v2[i].z = -1;
	}

*/





/*
float fun(float x, float yt)
{
	return yt*sin(log(x));
};

int percent(float Value, int per) {
	return  (int)Value*per / 100;
};
*/
char szClassName[] = "3D объект";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
	HWND hwnd;
	MSG messages;
	WNDCLASSEX wincl;

	wincl.hInstance = hInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = DCDemoGraphProc;
	wincl.style = CS_HREDRAW | CS_VREDRAW;
	wincl.cbSize = sizeof(WNDCLASSEX);

	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDC_ARROW);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;

	wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wincl.lpszClassName = szClassName;

	if (!RegisterClassEx(&wincl))
		return 0;
	hwnd = CreateWindowEx(0, szClassName, "3D объект", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nFunsterStil);
	UpdateWindow(hwnd);
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return messages.wParam;
}


LRESULT CALLBACK DCDemoGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y, x0, y0, z0;
	int Mx = 25;
	int My = 3;
	float focus = 0.5;
	HDC   hDC;
	Vector vec(7, 5, -4);// направления обзора
	Vector mv(0, 40, -40);// перемещение камеры в точку -mv
	Vector a1(5, 3, -1), b1(3, 5, -1), c1(5, 7, -1), d1(7, 5, -1), a2(5,3, -4), b2(3, 5, -4), c2(5, 7, -4), d2(7, 5, -4);
	Vector f(4, 2, -1);
	a1.VSumm(mv); a1.Image(focus, vec);
	b1.VSumm(mv); b1.Image(focus, vec);
	c1.VSumm(mv); c1.Image(focus, vec);
	d1.VSumm(mv); d1.Image(focus, vec);
	a2.VSumm(mv); a2.Image(focus, vec);
	b2.VSumm(mv); b2.Image(focus, vec);
	c2.VSumm(mv); c2.Image(focus, vec);
	d2.VSumm(mv); d2.Image(focus, vec);
	f.Image(focus, vec);
	int n = 1000;//Масштабирование в 1000 раз

	const int m = 1000;
	float b = 4;
	float dx = b / m;

	PAINTSTRUCT  PaintStruct;
	HANDLE hBM;
	RECT Rect;
	HPEN pen;
	HPEN Pens;

	GetWindowRect(hwnd, &Rect);
	int width = (Rect.right - Rect.left) / 2;
	int height = (Rect.bottom - Rect.top) / 2;
	Vector her(width, height, 0);//Вектор, определяющий расположение на экране
	Vector beg(0, 0, 0);
	Vector pidor = -mv + (vec ^ 2);
	switch (message)
	{
		case WM_PAINT:
			GetClientRect(hwnd, &Rect);
			hDC = BeginPaint(hwnd, &PaintStruct);
			pen = CreatePen(0, 1, RGB(0, 0, 0));
			SelectObject(hDC, pen);
/*
			//Parabola

			Vector *v1, *v2;
			v1 = new Vector[m];
			v2 = new Vector[m];

			//Right parabola wing
			for (int i = 0; i < m; i++) {
				v1[i].x = i*dx;
				v1[i].y = i*i*dx*dx;
				v1[i].z = -1;
			}
			//Left parabola wing

			for (int i = 0; i < m; i++) {
				v2[i].x = -i*dx;
				v2[i].y = i*i*dx*dx;
				v2[i].z = -1;
			}

			//DrawCLine(hDC, v1, m, n, vec, mv, her, focus);
			//DrawCLine(hDC, v2, m, n, vec, mv, her, focus);
*/
			DrawTor(hDC, vec, mv, her, pidor, focus, 5, 10, n);
			DrawTor(hDC, vec, mv, her, pidor + Vector(3, 7, 2), focus, 6, 0, n);

			//DrawTor(hDC, vec, mv, her, beg, focus, 5, 10, n);

			/*
			ICE_CUBE

			a1 = a1.VNumMult(n).VSumm(her);
			b1 = b1.VNumMult(n).VSumm(her);
			c1 = c1.VNumMult(n).VSumm(her);
			d1 = d1.VNumMult(n).VSumm(her);
			a2 = a2.VNumMult(n).VSumm(her);
			b2 = b2.VNumMult(n).VSumm(her);
			c2 = c2.VNumMult(n).VSumm(her);
			d2 = d2.VNumMult(n).VSumm(her);
			f = f.VNumMult(n).VSumm(her);
			
			//Lower area

			MoveToEx(hDC, int(a1.x), int(a1.y), NULL);
			LineTo(hDC, int(b1.x), int(b1.y));

			MoveToEx(hDC, int(b1.x), int(b1.y), NULL);
			LineTo(hDC, int(c1.x), int(c1.y));

			MoveToEx(hDC, int(c1.x), int(c1.y), NULL);
			LineTo(hDC, int(d1.x), int(d1.y));

			MoveToEx(hDC, int(d1.x), int(d1.y), NULL);
			LineTo(hDC, int(a1.x), int(a1.y));
			
			//Upper area

			MoveToEx(hDC, int(a2.x), int(a2.y), NULL);
			LineTo(hDC, int(b2.x), int(b2.y));

			MoveToEx(hDC, int(b2.x), int(b2.y), NULL);
			LineTo(hDC, int(c2.x), int(c2.y));

			MoveToEx(hDC, int(c2.x), int(c2.y), NULL);
			LineTo(hDC, int(d2.x), int(d2.y));

			MoveToEx(hDC, int(d2.x), int(d2.y), NULL);
			LineTo(hDC, int(a2.x), int(a2.y));
			
			//The sides

			MoveToEx(hDC, int(a1.x), int(a1.y), NULL);
			LineTo(hDC, int(a2.x), int(a2.y));

			MoveToEx(hDC, int(b1.x), int(b1.y), NULL);
			LineTo(hDC, int(b2.x), int(b2.y));

			MoveToEx(hDC, int(c1.x), int(c1.y), NULL);
			LineTo(hDC, int(c2.x), int(c2.y));

			MoveToEx(hDC, int(d1.x), int(d1.y), NULL);
			LineTo(hDC, int(d2.x), int(d2.y));

			/*
			// рисование осей координат
			x0 = percent(PaintStruct.rcPaint.right, 3);
			y0 = (PaintStruct.rcPaint.bottom - percent(PaintStruct.rcPaint.bottom, 3));
			MoveToEx(hDC, x0, y0 / 2, NULL);
			LineTo(hDC, PaintStruct.rcPaint.right - percent(PaintStruct.rcPaint.right, 3), y0 / 2);
			x = PaintStruct.rcPaint.right - percent(PaintStruct.rcPaint.right, 3);
			y = y0 / 2;
			LineTo(hDC, x - percent(PaintStruct.rcPaint.right, 4), y - percent(PaintStruct.rcPaint.bottom, 1));
			MoveToEx(hDC, x, y, NULL);
			LineTo(hDC, x - percent(PaintStruct.rcPaint.right, 4), y + percent(PaintStruct.rcPaint.bottom, 1));
			MoveToEx(hDC, x0, y0, NULL);
			LineTo(hDC, x0, PaintStruct.rcPaint.top + percent(PaintStruct.rcPaint.bottom, 3));
			x = x0;
			y = PaintStruct.rcPaint.top + percent(PaintStruct.rcPaint.bottom, 3);
			LineTo(hDC, x - percent(PaintStruct.rcPaint.right, 1), y + percent(PaintStruct.rcPaint.bottom, 4));
			MoveToEx(hDC, x, y, NULL);
			LineTo(hDC, x + percent(PaintStruct.rcPaint.right, 1), y + percent(PaintStruct.rcPaint.bottom, 4));
			y0 = y0 / 2;
			pen = CreatePen(0, 2, RGB(0xFF, 0, 0xFF));
			SelectObject(hDC, pen);
			// Масштаб
			dx = ((float)PaintStruct.rcPaint.right) / Mx;
			dy = ((float)PaintStruct.rcPaint.bottom) / My;
			curx = 0;
			cury = fun(curx, 1);
			x = x0 + curx * dx;
			y = y0 - cury * dy;
			MoveToEx(hDC, x, y, NULL);
			while (curx<Mx) {
				curx += 0.001;
				cury = fun(curx, 1);
				x = x0 + curx * dx;
				y = y0 - cury * dy;
				LineTo(hDC, x, y);
			}
			*/
			EndPaint(hwnd, &PaintStruct);
			//deleteObject(pen);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}