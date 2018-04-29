// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MemoryBitStream.h"


class ActionInputState
{
public:

	ActionInputState() :
		mDesiredMoveForwardAmount( 0.f ),
		mDesiredMoveRightAmount( 0.f ),

		mDesiredTurnAmountX( 0.f ),
		mDesiredTurnAmountY( 0.f ),
		mDesiredTurnAmountZ( 0.f ),

		mDesiredLookUpAmountX( 0.f ),
		mDesiredLookUpAmountY( 0.f ),
		mDesiredLookUpAmountZ( 0.f ),

		mDesiredTurnRateAmount( 0.f ),
		mDesiredLookUpRateAmount( 0.f ),
		mDesiredOnStartJumpAmount( 0.f ),
		mDesiredOnStopJumpAmount( 0.f ),
		mDesiredMoveUpAmount( 0.f ),
		mIsShooting( false )
	{}

	float GetDesiredMoveForwardAmount()	const { return mDesiredMoveForwardAmount; }
	float GetDesiredMoveRightAmount()	const { return mDesiredMoveRightAmount; }

	FRotator GetDesiredTurnRot()	const { return FRotator(mDesiredTurnAmountX, mDesiredTurnAmountY, mDesiredTurnAmountZ); }
	FRotator GetDesiredLookUpRot()	const { return FRotator(mDesiredLookUpAmountX, mDesiredLookUpAmountY, mDesiredLookUpAmountZ); }

	float GetDesiredTurnRateAmount()	const { return mDesiredTurnRateAmount; }
	float GetDesiredLookUpRateAmount()	const { return mDesiredLookUpRateAmount; }

	float GetDesiredOnStartJumpAmount()	const { return mDesiredOnStartJumpAmount; }
	float GetDesiredOnStopJumpAmount()	const { return mDesiredOnStopJumpAmount; }

	float GetDesiredOnMoveUpAmount()	const { return mDesiredMoveUpAmount; }

	bool  IsShooting()					const { return mIsShooting; }

	bool Write( OutputMemoryBitStream& inOutputStream ) const;
	bool Read( InputMemoryBitStream& inInputStream );

private:
	friend class InputManager;

	float	mDesiredTurnAmountX;
	float	mDesiredTurnAmountY;
	float	mDesiredTurnAmountZ;

	float   mDesiredLookUpAmountX;
	float   mDesiredLookUpAmountY;
	float   mDesiredLookUpAmountZ;

	float	mDesiredMoveRightAmount;
	float	mDesiredMoveForwardAmount;

	float	mDesiredTurnRateAmount;
	float	mDesiredLookUpRateAmount;

	float	mDesiredOnStartJumpAmount;
	float	mDesiredOnStopJumpAmount;

	float	mDesiredMoveUpAmount;

	bool	mIsShooting;
};