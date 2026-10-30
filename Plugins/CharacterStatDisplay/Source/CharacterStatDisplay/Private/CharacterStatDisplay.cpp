// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterStatDisplay.h"
#include "CharacterStatDisplayStyle.h"
#include "CharacterStatDisplayCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "UObject/UnrealType.h"
#include "UObject/TextProperty.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Views/STableRow.h"

static const FName CharacterStatDisplayTabName("CharacterStatDisplay");

#define LOCTEXT_NAMESPACE "FCharacterStatDisplayModule"

static TSharedRef<ITableRow> GenerateCombatantRow(
	TSharedPtr<FCombatantRow> Item, const TSharedRef<STableViewBase>& Owner);

void FCharacterStatDisplayModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCharacterStatDisplayStyle::Initialize();
	FCharacterStatDisplayStyle::ReloadTextures();

	FCharacterStatDisplayCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCharacterStatDisplayCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FCharacterStatDisplayModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCharacterStatDisplayModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CharacterStatDisplayTabName, FOnSpawnTab::CreateRaw(this, &FCharacterStatDisplayModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FCharacterStatDisplayTabTitle", "CharacterStatDisplay"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	TickHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateRaw(this, &FCharacterStatDisplayModule::OnTick), 0.5f);
}

void FCharacterStatDisplayModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCharacterStatDisplayStyle::Shutdown();

	FCharacterStatDisplayCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CharacterStatDisplayTabName);

	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
	ListView.Reset();
	Rows.Reset();
}

TSharedRef<SDockTab> FCharacterStatDisplayModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	SAssignNew(ListView, SListView<TSharedPtr<FCombatantRow>>)
		.ListItemsSource(&Rows)
		.SelectionMode(ESelectionMode::None)
		.OnGenerateRow_Static(&GenerateCombatantRow);

	RefreshRows();

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight().Padding(4)
				[
					SNew(SButton)
						.Text(FText::FromString(TEXT("Refresh")))
						.OnClicked_Lambda([this]()
							{
								RefreshRows();
								return FReply::Handled();
							})
				]
			+ SVerticalBox::Slot().FillHeight(1.f)
				[
					ListView.ToSharedRef()
				]
		];
}

void FCharacterStatDisplayModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(CharacterStatDisplayTabName);
}

void FCharacterStatDisplayModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCharacterStatDisplayCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCharacterStatDisplayCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

//ACTUAL STAT FUNCTION STARTS HERE

static FProperty* FindStructField(const UStruct* S, const TCHAR* AuthoredName)
{
	for (TFieldIterator<FProperty> It(S); It; ++It)
	{
		if (It->GetAuthoredName() == AuthoredName)
		{
			return *It;
		}
	}
	return nullptr;
}

static double ReadStructNumber(const UStruct* S, const void* Data, const TCHAR* Field)
{
	if (FProperty* P = FindStructField(S, Field))
	{
		if (FNumericProperty* N = CastField<FNumericProperty>(P))
		{
			return N->GetFloatingPointPropertyValue(P->ContainerPtrToValuePtr<void>(Data));
		}
	}
	return 0.0;
}

static void ReadAttributes(UObject* Comp, FCombatantRow& Row)
{
	// The component's own variable: a normal name lookup works here
	FArrayProperty* Arr = CastField<FArrayProperty>(
		Comp->GetClass()->FindPropertyByName(TEXT("Attribute")));
	if (!Arr) return;

	FStructProperty* AttrProp = CastField<FStructProperty>(Arr->Inner);
	if (!AttrProp) return;

	FProperty* NameP = FindStructField(AttrProp->Struct, TEXT("Name"));
	FProperty* DispP = FindStructField(AttrProp->Struct, TEXT("DisplayName"));
	FStructProperty* StatP = CastField<FStructProperty>(
		FindStructField(AttrProp->Struct, TEXT("Stat")));
	if (!StatP) return;

	FScriptArrayHelper Helper(Arr, Arr->ContainerPtrToValuePtr<void>(Comp));
	for (int32 i = 0; i < Helper.Num(); ++i)
	{
		const void* Elem = Helper.GetRawPtr(i);
		FStatLine L;

		FString NameStr, DispStr;
		if (FNameProperty* NP = CastField<FNameProperty>(NameP))
			NameStr = NP->GetPropertyValue(NP->ContainerPtrToValuePtr<void>(Elem)).ToString();
		if (FTextProperty* TP = CastField<FTextProperty>(DispP))
			DispStr = TP->GetPropertyValue(TP->ContainerPtrToValuePtr<void>(Elem)).ToString();
		L.Label = DispStr.IsEmpty() ? NameStr : DispStr;

		const void* StatData = StatP->ContainerPtrToValuePtr<void>(Elem);
		const UStruct* SS = StatP->Struct;
		L.Min = ReadStructNumber(SS, StatData, TEXT("MinValue"));
		L.MinMod = ReadStructNumber(SS, StatData, TEXT("MinValueMod"));
		L.Max = ReadStructNumber(SS, StatData, TEXT("MaxValue"));
		L.MaxMod = ReadStructNumber(SS, StatData, TEXT("MaxValueMod"));
		L.Cur = ReadStructNumber(SS, StatData, TEXT("CurrentValue"));
		L.CurMod = ReadStructNumber(SS, StatData, TEXT("CurrentValueMod"));

		Row.Lines.Add(L);
	}
}

static TSharedRef<ITableRow> GenerateCombatantRow(
	TSharedPtr<FCombatantRow> Item, const TSharedRef<STableViewBase>& Owner)
{
	TSharedRef<SVerticalBox> Box = SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 6, 0, 2)
		[
			SNew(STextBlock)
				.Text(FText::FromString(Item->ActorName))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		];

	for (const FStatLine& L : Item->Lines)
	{
		const double EffectiveCur = L.Cur + L.CurMod;
		const double EffectiveMax = L.Max + L.MaxMod;
		const double EffectiveMin = L.Min + L.MinMod;

		const FString Text = FString::Printf(
			TEXT("%s:  %.2f / %.2f   (min %.2f)    mods: cur %+.2f, max %+.2f, min %+.2f"),
			*L.Label, EffectiveCur, EffectiveMax, EffectiveMin, L.CurMod, L.MaxMod, L.MinMod);

		Box->AddSlot().AutoHeight().Padding(16, 0, 0, 0)
			[
				SNew(STextBlock).Text(FText::FromString(Text))
			];
	}

	return SNew(STableRow<TSharedPtr<FCombatantRow>>, Owner)[Box];
}

UWorld* FCharacterStatDisplayModule::GetTargetWorld() const
{
	if (!GEditor) return nullptr;
	if (GEditor->PlayWorld) return GEditor->PlayWorld;
	return GEditor->GetEditorWorldContext().World();
}

UClass* FCharacterStatDisplayModule::GetCompClass()
{
	if (!CachedCompClass.IsValid())
	{
		CachedCompClass = StaticLoadClass(
			UActorComponent::StaticClass(), nullptr,
			TEXT("/Game/TopDown/AnsonStuff/Turnbased/Blueprints/AbilitySystemComponent.AbilitySystemComponent_C")); //HAVE TO CHANGE THE PATH IF IT IS CHANGED, also must have _C
	}
	return CachedCompClass.Get();
}

void FCharacterStatDisplayModule::RefreshRows()
{
	Rows.Reset();

	UWorld* World = GetTargetWorld();
	UClass* CompClass = GetCompClass();

	if (World && CompClass)
	{
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			UActorComponent* Comp = It->FindComponentByClass(CompClass);
			if (!Comp) continue;

			TSharedPtr<FCombatantRow> Row = MakeShared<FCombatantRow>();
			Row->ActorName = It->GetActorNameOrLabel();
			ReadAttributes(Comp, *Row);
			Rows.Add(Row);
		}
	}

	if (ListView.IsValid())
	{
		ListView->RequestListRefresh();
	}
}

bool FCharacterStatDisplayModule::OnTick(float)
{
	if (ListView.IsValid() && GEditor && GEditor->PlayWorld)
	{
		RefreshRows();
	}
	return true;   // keep ticking
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCharacterStatDisplayModule, CharacterStatDisplay)