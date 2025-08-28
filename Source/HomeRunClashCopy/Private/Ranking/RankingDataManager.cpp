// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranking/RankingDataManager.h"

#include "JsonObjectConverter.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

const FString RankingDataManager::URL= TEXT("https://homerun-fa203-default-rtdb.firebaseio.com/ranking.json");
FOnRankingLoaded RankingDataManager::OnRankingLoaded;

FRankingArray RankingDataManager::LoadLocal(const FString& FileName)
{
	FRankingArray RankData;
	FString JsonStr;
	if (FFileHelper::LoadFileToString(JsonStr, *(FPaths::ProjectSavedDir() / FileName)))
	{
		if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonStr, &RankData, 0, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON into FRankingDataArray"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ranking.json not found, returning empty ranking array"));
	}
	
	return RankData;
}

void RankingDataManager::SaveLocal(const FRankingArray& Data, const FString& FileName)
{
	FString OutputString;
	FJsonObjectConverter::UStructToJsonObjectString(Data, OutputString);

	FFileHelper::SaveStringToFile(OutputString, *(FPaths::ProjectSavedDir() / FileName));
}

void RankingDataManager::AddNewDataLocal(FRankingArray& Data, const FString& Name, const int32 Score, const int32 ThrowBall, const FString& FileName)
{
	FRankingData NewData { Name, Score, ThrowBall};
	Data.Data.Push(NewData);
	
	Data.Data.Sort([](const FRankingData& A, const FRankingData& B)
	{
		if (A.ThrowBallCount == B.ThrowBallCount)
			return A.Score > B.Score;

		return A.ThrowBallCount < B.ThrowBallCount;
	});
	
	// 상위 10개만 유지
	// if (Data.Num() > 10)
	// {
	// 	Data.SetNum(10);
	// }
	
	SaveLocal(Data, FileName);
}



void RankingDataManager::LoadOnline()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->OnProcessRequestComplete().BindStatic(&RankingDataManager::OnLoadRankingResponse);
	Request->ProcessRequest();
}

void RankingDataManager::SaveOnline(const FRankingData& Data)
{
	// Json 변환
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("Name"), Data.Name);
	JsonObject->SetNumberField(TEXT("Score"), Data.Score);
	JsonObject->SetNumberField(TEXT("ThrowBallCount"), Data.ThrowBallCount);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// HTTP 요청
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("POST");  // POST: push new entry, PUT: overwrite
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->OnProcessRequestComplete().BindStatic(&RankingDataManager::OnSaveRankingResponse);
	Request->ProcessRequest();
}

void RankingDataManager::OnLoadRankingResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray<FRankingData> RankingArray;
	
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("LoadRanking failed"));
		return;
	}

	if (Response->GetResponseCode() == 200)
	{
		FString ResponseString = Response->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("LoadRanking Success: %s"), *ResponseString);

		// JSON 파싱
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			for (auto& Elem : JsonObject->Values)
			{
				TSharedPtr<FJsonObject> ChildObject = Elem.Value->AsObject();
				if (ChildObject.IsValid())
				{
					FRankingData Data;
					Data.Name = ChildObject->GetStringField(TEXT("Name"));
					Data.Score = ChildObject->GetIntegerField(TEXT("Score"));
					Data.ThrowBallCount = ChildObject->GetIntegerField(TEXT("ThrowBallCount"));
					RankingArray.Add(Data);

					UE_LOG(LogTemp, Log, TEXT("Ranking: %s | Score: %d | Throw: %d"),
						*Data.Name, Data.Score, Data.ThrowBallCount);
				}
			}
		}

		OnRankingLoaded.ExecuteIfBound(RankingArray);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LoadRanking Error Code: %d"), Response->GetResponseCode());
	}
}

void RankingDataManager::OnSaveRankingResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SaveRanking failed"));
		return;
	}
	
	if (bWasSuccessful && Response->GetResponseCode() == 200)
	{
		UE_LOG(LogTemp, Log, TEXT("Upload Success: %s"), *Response->GetContentAsString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Upload Failed"));
	}
}
