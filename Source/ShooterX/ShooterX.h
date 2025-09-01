// ShooterX.h

#pragma once

#include "CoreMinimal.h"

SHOOTERX_API DECLARE_LOG_CATEGORY_EXTERN(LogSX, Log, All);
SHOOTERX_API DECLARE_LOG_CATEGORY_EXTERN(LogSXNet, Log, All);

#pragma region NetLogging

#define NETMODE_TCHAR ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))
#define SX_LOG_NETMODE(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("[%s] %s %s"), NETMODE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))

#define LOCAL_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define REMOTE_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define SX_LOG_NETROLE(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), NETMODE_TCHAR, LOCAL_ROLE_TCHAR, REMOTE_ROLE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))

#define LOCAL_ROLE_SUBOBJECT_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))
#define REMOTE_ROLE_SUBOBJECT_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))
#define SX_LOG_NETROLE_SUBOBJECT(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), NETMODE_TCHAR, LOCAL_ROLE_SUBOBJECT_TCHAR, REMOTE_ROLE_SUBOBJECT_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))

#pragma endregion

class SXUtilityFunctionLibrary
{
public:
    static void PrintString(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay = 1.f, FColor InColor = FColor::Cyan)
    {
        if (IsValid(GEngine) == true && IsValid(InWorldContextActor) == true)
        {
            if (InWorldContextActor->GetNetMode() == NM_Client)
            {
                GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
            }
        }
    }

    static FString GetNetModeString(const AActor* InWorldContextActor)
    {
        FString NetModeString = TEXT("None");

        if (IsValid(InWorldContextActor) == true)
        {
            ENetMode NetMode = InWorldContextActor->GetNetMode();
            if (NetMode == NM_Client)
            {
                NetModeString = TEXT("Client");
            }
            else
            {
                if (NetMode == NM_Standalone)
                {
                    NetModeString = TEXT("StandAlone");
                }
                else
                {
                    NetModeString = TEXT("Server");
                }
            }
        }

        return NetModeString;
    }

    static FString GetNetRoleString(const AActor* InActor)
    {
        FString RoleString = TEXT("None");

        if (IsValid(InActor) == true)
        {
            FString LocalRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetLocalRole());
            FString RemoteRoleString = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetRemoteRole());

            RoleString = FString::Printf(TEXT("%s / %s"), *LocalRoleString, *RemoteRoleString);
        }

        return RoleString;
    }

};