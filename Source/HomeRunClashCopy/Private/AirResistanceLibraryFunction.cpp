// Fill out your copyright notice in the Description page of Project Settings.


#include "AirResistanceLibraryFunction.h"


FVector UAirResistanceLibraryFunction::AirResistanceCpp(FVector Velocity, UCurveFloat* DragCoefficient)
{
	float Cd = DragCoefficient->GetFloatValue(Velocity.Length());
	FVector DragVelocity;

	DragVelocity.X = pow(Velocity.X, 2) * Cd;
	DragVelocity.Y = pow(Velocity.Y, 2) * Cd;
	DragVelocity.Z = pow(Velocity.Z, 2) * Cd;

	FVector VelocityWithDrag = Velocity - (DragVelocity / 1000000.0);	// 1000000.0: Drag Coefficient Parameter
	
	return VelocityWithDrag;
	
}