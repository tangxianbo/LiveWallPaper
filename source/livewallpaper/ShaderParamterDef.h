#pragma once

class ShaderParameterDef
{
public:
	ShaderParameterDef(int location, int arraySize, int valueType):mLocation(location)
																,mArraySize(arraySize)
																,mValueType(valueType)
	{
	}
	~ShaderParameterDef()
	{
	}

public:
	int GetLocation();
	int GetArraySize();
	int GetValueType();

private:
	int		mLocation;
	int		mArraySize;
	int		mValueType;
};

inline int 
ShaderParameterDef::GetLocation()
{
	return mLocation;
}

inline int 
ShaderParameterDef::GetArraySize()
{
	return mArraySize;
}

inline int 
ShaderParameterDef::GetValueType()
{
	return mValueType;
}