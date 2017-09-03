/*
	동적 2차원 배열
	
	작성자 서지민
*/
class Dynamic_Matrix
{
	public:
	
	int Width,Height;
	int Window_Size;
	
	double **Mat;
	
	Dynamic_Matrix()
	{
		
	}
	
	~Dynamic_Matrix()
	{
		for(int i=0;i<Height;i++)
		{
			delete [] Mat[i];
		}
		delete []Mat;
	}
	
	void Build(int Window_Size)
	{
		this->Width = Window_Size;
		this->Height = Window_Size;
		this->Window_Size = Window_Size;
		
		Mat = new double *[Window_Size];
		
		for(int i=0;i<Window_Size;i++)
		{
			Mat[i] = new double[Window_Size];
		}
		
		for(int i=0;i<Window_Size;i++)
		{
			for(int j=0;j<Window_Size;j++)
			{
				Mat[i][j] = 0;
			}
		}
	}
	
	void Build(int Width,int Height)
	{
		this->Width = Window_Size;
		this->Height = Window_Size;
		
		Mat = new double *[Height];
		
		for(int i=0;i<Height;i++)
		{
			Mat[i] = new double[Width];
		}
		
		for(int i=0;i<Height;i++)
		{
			for(int j=0;j<Width;j++)
			{
				Mat[i][j] = 0;
			}
		}
	}
	
	void Set_Value(int i,int j, double Data)
	{
		Mat[i][j] = Data;
	}
	
	double Get_Value(int i,int j)
	{
		return Mat[i][j];
	}
	
	double** Get_Matrix()
	{
		return Mat;
	}
};