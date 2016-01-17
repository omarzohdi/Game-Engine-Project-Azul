#include "AnimationData.h"
#include "File.h"
#include "MathEngine.h"
#include <assert.h>

AnimationData::AnimationData()
{
	pHead = 0;
}

AnimationData::AnimationData(char * filename)
{
	totalAnimations = 0;
	LoadDatafromFile(filename);
	pHead = this->pHeads[0];
}

Frame_Bucket * AnimationData::getHead()
{
	return this->pHead;
}
Frame_Bucket * AnimationData::getHead(int offset)
{
	return this->pHeads[offset];
}
Frame_Bucket * AnimationData::getNext(Frame_Bucket * pTmp)
{
	return pTmp->nextBucket;
}

Frame_Bucket * AnimationData::getPrev(Frame_Bucket * pTmp)
{
	return pTmp->prevBucket;
}
void AnimationData::LoadAnimationData(FileHandle &fh, int inIndex)
{
	FileError ferror;
	int *tFrames = new int();
	float *bonedata = new float();

	ferror=File::read(fh,tFrames,sizeof(int));
	assert(ferror == FILE_SUCCESS);

	for (int i=0; i < *tFrames; i++)
	{
		Frame_Bucket *pTmp = new Frame_Bucket();
		pTmp->KeyTime = i * Time(TIME_NTSC_30_FRAME);
		pTmp->pBone = new Bone[NUM_BONES];
		pTmp->nextBucket = 0;
		pTmp->prevBucket = 0;

		for (int j =0; j<NUM_BONES; j++)
		{
			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			pTmp->pBone[j].T[X] = *bonedata;
			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			pTmp->pBone[j].T[Y] = *bonedata;
			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			pTmp->pBone[j].T[Z] = *bonedata;

			Vect tempV;

			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			tempV[X] = *bonedata;
			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			tempV[Y] = *bonedata;
			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			tempV[Z] = *bonedata;

			pTmp->pBone[j].Q = Quat(ROT_XYZ, tempV[X],tempV[Y],tempV[Z]);

			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			pTmp->pBone[j].S[X] = *bonedata;
			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			pTmp->pBone[j].S[Y] = *bonedata;
			ferror = File::read(fh,bonedata,sizeof(float));
			assert(ferror == FILE_SUCCESS);
			pTmp->pBone[j].S[Z] = *bonedata;

		}

		if (i == 0)
		{
			pHeads[inIndex]->nextBucket = pTmp;
			pTmp->prevBucket = pHead;
			currFrame = pTmp;
		}
		else
		{
			pTmp->prevBucket = currFrame;
			currFrame->nextBucket = pTmp;

			currFrame = pTmp;
		}
	}


	delete tFrames;
	delete bonedata;
}

void AnimationData::LoadDatafromFile(char * filename)
{
	FileHandle fh;
	FileError ferror;
	PackageHeader * pkH = new PackageHeader();
	ChunkHeader * ckH = new ChunkHeader();
	ChunkHeader * ckH2 = new ChunkHeader();

	ferror = File::open(fh,filename,FILE_READ);
	assert(ferror == FILE_SUCCESS);

	ferror= File::read(fh,pkH,sizeof(PackageHeader));
	assert(ferror == FILE_SUCCESS);

	totalAnimations = pkH->numChunks;
	pHeads = new Frame_Bucket*[ totalAnimations ];

	for (int i=0 ; i < totalAnimations ; i++)
	{
		ferror= File::read(fh,ckH,sizeof(ChunkHeader));
		assert(ferror == FILE_SUCCESS);

		int remainder = ckH->chunkSize % 32;
		int * correctStringLength = new int(ckH->chunkSize + 32 - remainder);

		pHeads[i] = new Frame_Bucket();
		pHeads[i]->prevBucket = 0;
		pHeads[i]->nextBucket = 0;
		pHeads[i]->KeyTime = Time(TIME_ZERO);
		pHeads[i]->pBone = new Bone[NUM_BONES];

		LoadAnimationData(fh, i);

		ferror= File::seek(fh,FILE_SEEK_BEGIN,ckH->chunkSize+80);
		assert(ferror == FILE_SUCCESS);
	}

	delete	pkH;
	delete  ckH;		
}