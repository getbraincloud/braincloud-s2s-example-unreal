// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "S2SRTTComms.h"
#include <BCClientPlugin/Private/WinWebSocketBase.h>
#include "GameFramework/Actor.h"
#include "Engine/EngineBaseTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DedicatedDemoGameMode.generated.h"



DECLARE_DYNAMIC_DELEGATE_OneParam(FS2SCallbackDelegate, const FString&, OutputString);
DECLARE_DYNAMIC_DELEGATE_OneParam(FS2SRTTCallbackDelegate, const FString&, JsonData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRSMSocketClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRSMSocketConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSMSocketReceiveData, const TArray<uint8>&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSMSocketReceiveMessage, const FString&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRSMSocketConnectError, const FString&, error);

DECLARE_LOG_CATEGORY_EXTERN(DedicatedServerLog, Log, All);

UENUM(BlueprintType)
enum BCNetMode {
	Standalone,
	DedicatedServer,
	ListenServer,
	Client,
	MAX
};

UCLASS(minimalapi)
class ADedicatedDemoGameMode : public AGameModeBase, public IWinWebSocketBaseCallbacks
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void InitS2S(const FString& appId, const FString& serverName, const FString& serverSecret);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud Room Server Manager Tool")
		void ConnectToRSMTool();

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void S2SRequest(const FString& requestJson, FS2SCallbackDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void EnableRTT(FS2SCallbackDelegate OnSuccess, FS2SCallbackDelegate OnFailure);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void RegisterS2SRTTCallback(FS2SRTTCallbackDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void JoinSystemChannel(FS2SRTTCallbackDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void DeregisterS2SRTTCallback();

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void RunCallbacks();

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void CloseWebsocketConnection();

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void ShutdownServer();
		
	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		BCNetMode GetNetModeEnum() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BrainCloud S2S")
		void OnLobbyAssigned();

	void OnLobbyAssigned_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BrainCloud S2S")
		void OnRSMConnectError(const FString& message);

	void OnRSMConnectError_Implementation(const FString& message);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BrainCloud S2S")
		void OnRSMConnectComplete();

	void OnRSMConnectComplete_Implementation();

	UPROPERTY(BlueprintReadOnly)
		FString lobbyId;

	void OnDeploymentDeleteResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

private:

	void setupWebSocket(const FString& in_url);

	void OnConnectError(const FString& error) override;
	void OnClosed() override;
	void OnConnectComplete() override;
	void OnReceiveData(const TArray<uint8>& data) override;
	
	void OnDataProcessed(const FString& message);
	

	US2SRTTComms *pS2S;

	UWinWebSocketBase* m_connectedSocket;

	FString serverName;
	FString serverSecret;
	FString appId;
	bool S2SInitialized = false;
};





