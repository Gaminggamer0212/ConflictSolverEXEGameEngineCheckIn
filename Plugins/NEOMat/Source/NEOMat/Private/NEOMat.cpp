// Copyright Epic Games, Inc. All Rights Reserved.

#include "NEOMat.h"

#include "ToolMenus.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "Misc/MessageDialog.h"

// unreals ui for the widget
#include "Widgets/SWindow.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "FNEOMatModule"

void FNEOMatModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNEOMatModule::RegisterMenus));
}

void FNEOMatModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

void FNEOMatModule::InitializeEffects()
	{
		EffectOptions.Empty();
		EffectOptions.Add(MakeShared<FString>(TEXT("Neon")));
		EffectOptions.Add(MakeShared<FString>(TEXT("Glitch")));

		SelectedEffect = EffectOptions[0];
	}

void FNEOMatModule::OpenEffectWin()
	{
		InitializeEffects();

		EffectWindow = SNew(SWindow)
			.Title(FText::FromString("NEOMat - Choose your Effect"))
			.ClientSize(FVector2D(420, 670))
			.SupportsMinimize(false)
			.SupportsMaximize(false);

		EffectWindow->SetContent(SNew(SVerticalBox)

		+ SVerticalBox::Slot().AutoHeight().Padding(12)
			[
				SNew(STextBlock).Text(FText::FromString("Choose an effect:"))
			]

		+ SVerticalBox::Slot().AutoHeight().Padding(12)
			[
				SNew(SComboBox<TSharedPtr<FString>>).OptionsSource(&EffectOptions).OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
					{
						return SNew(STextBlock).Text(FText::FromString(*InItem));
					}).OnSelectionChanged_Lambda([this](TSharedPtr<FString> Item, ESelectInfo::Type)
						{
							if (Item.IsValid())
							{
								SelectedEffect = Item;
							}
						})

						[
							SNew(STextBlock).Text_Lambda([this]()
								{
									return FText::FromString(SelectedEffect.IsValid() ? *SelectedEffect : TEXT("Select effect"));
								})
						]
			]

		+ SVerticalBox::Slot().AutoHeight().Padding(12)
			[
				SNew(STextBlock).Text_Lambda([this]()
					{
						return FText::Format(FText::FromString(TEXT("Effect strength: {0}")), FText::AsNumber(SelectStrength));
					})
			]

		+ SVerticalBox::Slot().AutoHeight().Padding(12)
			[
				SNew(SSlider).Value_Lambda([this]()
					{
						return SelectStrength;
					}).OnValueChanged_Lambda([this](float Val)
						{
							SelectStrength = Val;
						})
			]

		+ SVerticalBox::Slot().AutoHeight().Padding(12)
			[
				SNew(STextBlock).Text(FText::FromString("Effect color:"))
			]

		+ SVerticalBox::Slot().AutoHeight().Padding(12)
			[
				SNew(SColorBlock).Color_Lambda([this]()
					{
						return SelectColor;
					}).Size(FVector2D(100.0f, 25.0f))
			]

		+ SVerticalBox::Slot().AutoHeight().Padding(6)
			[
				SNew(STextBlock).Text(FText::FromString("R"))
			]
			
		+ SVerticalBox::Slot().AutoHeight().Padding(6)
			[
				SNew(SSlider)
					.Value_Lambda([this]() { return SelectColor.R; })
					.OnValueChanged_Lambda([this](float Value)
						{
							SelectColor.R = Value;
						})
			]



		+SVerticalBox::Slot().AutoHeight().Padding(6)
			[
				SNew(STextBlock).Text(FText::FromString("G"))
			]

			+ SVerticalBox::Slot().AutoHeight().Padding(6)
			[
				SNew(SSlider)
					.Value_Lambda([this]() { return SelectColor.G; })
					.OnValueChanged_Lambda([this](float Value)
						{
							SelectColor.G = Value;
						})
			]



		+SVerticalBox::Slot().AutoHeight().Padding(6)
			[
				SNew(STextBlock).Text(FText::FromString("B"))
			]

			+ SVerticalBox::Slot().AutoHeight().Padding(6)
			[
				SNew(SSlider)
					.Value_Lambda([this]() { return SelectColor.B; })
					.OnValueChanged_Lambda([this](float Value)
						{
							SelectColor.B = Value;
						})
			]

		+ SVerticalBox::Slot().AutoHeight().Padding(12)
			[
				SNew(SButton).Text(FText::FromString("Generate and Apply")).OnClicked_Lambda([this]()
					{
						if (SelectedEffect.IsValid())
						{
							GenerateMaterial(*SelectedEffect);
						}

						return FReply::Handled();
					})
			]
			);

		FSlateApplication::Get().AddWindow(EffectWindow.ToSharedRef());
	}



void FNEOMatModule::RegisterMenus()
	{
		FToolMenuOwnerScoped OwnerScoped(this);

		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");

		FToolMenuSection& Section = Menu->FindOrAddSection("NEOMat");

		Section.AddMenuEntry("OpenNEOMat", LOCTEXT("OpenNEOMatLabel", "NEOMat Effects"), LOCTEXT("OpenNEOMatTooltip", "Choose and apply preset material effects!"), FSlateIcon(), FUIAction(FExecuteAction::CreateRaw(this, &FNEOMatModule::OpenEffectWin)));
	}

void FNEOMatModule::GenerateMaterial(const FString& EffectName)
	{
		if (!GEditor)
		{
			return;
		}




		// getting the parent mat from browser
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		TArray<FAssetData> SelectedAssets;

		ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

		UMaterialInterface* ParentMaterial = nullptr;

		for (const FAssetData& AssetData : SelectedAssets)
		{
			ParentMaterial = Cast<UMaterialInterface>(AssetData.GetAsset());

			if (ParentMaterial)
			{
				break;
			}
		}

		if (!ParentMaterial)
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("NoMaterialSelected", "Pleas select M_NEOMat material in the Content Browser... Pls"));

			return;
		}




		// make anew folder to put the new materials in
		const FString NEOMatFolder = TEXT("/Game/CharredStuff/Generated_NEOMats");




		// create the asset
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

		IAssetTools& AssetTools = AssetToolsModule.Get();

		FString NewMaterialName, AssetName;
		
		const FString EffectSuffix = (EffectName == "Neon") ? TEXT("_Neon") : TEXT("_Glitch");

		AssetTools.CreateUniqueAssetName(NEOMatFolder + TEXT("/M_NEOMat_") + EffectSuffix, TEXT(""), NewMaterialName, AssetName);

		const FString PackagePath = FPackageName::GetLongPackagePath(NewMaterialName);




		// set the selected material
		UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();

		Factory->InitialParent = ParentMaterial;

		UMaterialInstanceConstant* NewMaterial =
			Cast<UMaterialInstanceConstant>(AssetTools.CreateAsset(AssetName,PackagePath, UMaterialInstanceConstant::StaticClass(), Factory));


		if (!NewMaterial)
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("MaterialCreationFailed", "Can't creat material instamce :("));

			return;
		}




		// apply neon material and glowiness, set whats in the material editor (the params)
		if (EffectName == TEXT("Neon"))
		{
			NewMaterial->SetVectorParameterValueEditorOnly(FName("GlowColor"), SelectColor);
			NewMaterial->SetScalarParameterValueEditorOnly(FName("GlowStrength"), SelectStrength);
		}

		else if (EffectName == TEXT("Glitch"))
		{
			NewMaterial->SetVectorParameterValueEditorOnly(FName("GlowColor"), SelectColor);
			NewMaterial->SetScalarParameterValueEditorOnly(FName("GlowStrength"), SelectStrength);
			NewMaterial->SetScalarParameterValueEditorOnly(FName("GlitchStrength"), SelectStrength);
		}

		NewMaterial->PostEditChange(); // update the material in  editor
		NewMaterial->MarkPackageDirty(); //dirty basically marks the package as modified, saves when you saved the projsect



		// Apply material to selected object
		int32 AppliedCount = 0; // counter for how many this is apply to

		for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
		{
			AActor* Actor = Cast<AActor>(*It);

			if (!Actor)
			{
				continue;
			}

			TArray<UPrimitiveComponent*> Components; // grabing the property of actor, aka includes material slot

			Actor->GetComponents(Components);
				
			for (UPrimitiveComponent* Component : Components)
			{
				if (!Component)
				{
					continue;
				}

				const int32 MaterialSlots = Component->GetNumMaterials();

				for (int32 Slot = 0; Slot < MaterialSlots; ++Slot)
				{
					Component->SetMaterial(Slot, Cast<UMaterialInterface>(NewMaterial));
				}

				if (MaterialSlots > 0)
				{
					++AppliedCount;
				}
			}
		}

		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("Success!", "Created {0} and applied to {1} components. Remember to SAVE!!!"),FText::FromString(AssetName), FText::AsNumber(AppliedCount)));
	}

//void FNEOMatModule::StartupModule()
//{
//	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//}
//
//void FNEOMatModule::ShutdownModule()
//{
//	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
//	// we call this function before unloading the module.
//}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNEOMatModule, NEOMat)