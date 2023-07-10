

#pragma once

/*
Example Usages:

PRINT("Something");
PRINT(TEXT("something"));
 
COLORED_PRINT(CUR_CLASS_FUNC, FColor::Black);
COLORED_PRINT(CUR_CLASS_LINE, FColor::Black);
COLORED_PRINT(CUR_FUNC_SIG, FColor::Black);

PRINT_CLASS_LINE("my message");
PRINT_CLASS_LINE2("my message 1", "my message 2");
PRINT_CLASS_LINE_FLOAT("my message", 99.f);

int32 Health = 100;
float ArmorPct = 52.33;
FVector Location(33,12,1);	
LOG_MULTI("Health: %d, ArmorPct: %f, Loc: %s",  Health, ArmorPct, *Location.ToString());

Known Issues:
TEXT macros can cause issue due to its L identifier

 */

// Print text. PRINT("Something") or PRINT(TEXT("something"))
#define PRINT(Text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, Text, false)

// Define the color yourself
#define COLORED_PRINT(Text, Color) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Text, false) 


//Current Class Name + Function Name where this is called
#define CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called
#define CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called
#define CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
  
//Current Line Number in the code where this is called
#define CUR_LINE (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called
#define CUR_CLASS_LINE (CUR_CLASS + "(" + CUR_LINE + ")")
  
//Current Function Signature where this is called
#define CUR_FUNC_SIG (FString(__FUNCSIG__))

//Current Class Name + Function Name + Line Number where this is called
#define CUR_CLASS_FUNC_LINE (CUR_CLASS_FUNC + "(" + CUR_LINE + ")")

#define CUR_ACTOR_CLASS_FUNC_LINE ("[" + GetName() + "] => " + CUR_CLASS_FUNC_LINE)



// 	Gives you the Class name and exact line number where you print a message to yourself!**
#define PRINT_CLASS_LINE(Text) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(CUR_CLASS_FUNC_LINE + ": " + Text)), false )

// can print two parameters
#define PRINT_CLASS_LINE2(Text1, Text2) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(CUR_CLASS_FUNC_LINE + ": " + Text1 + " " + Text2)), false )

// can print a float with message
#define PRINT_CLASS_LINE_FLOAT(Text, Float) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(CUR_CLASS_FUNC_LINE + ": " + Text + " " + FString::SanitizeFloat(Float))), false )




//UE_LOG Messages. LogCat is LogCategory.
#define LOG(Text) 		   UE_LOG(LogTemp, Warning,TEXT("%s: %s"), *CUR_CLASS_FUNC_LINE, *FString(Text))
 
#define LOG2(Text1, Text2) 	UE_LOG(LogTemp, Warning,TEXT("%s: %s %s"), *CUR_CLASS_FUNC_LINE, *FString(Text1),*FString(Text2))

#define LOG_ACTOR(Text) 		  UE_LOG(LogTemp, Warning,TEXT("%s: %s"), *CUR_ACTOR_CLASS_FUNC_LINE, *FString(Text))

#define LOG_ERROR(Text) 		 UE_LOG(LogTemp,Error,TEXT("%s: %s"), *CUR_CLASS_FUNC_LINE, *FString(Text))

#define LOG_ACTOR_ERROR(Text) UE_LOG(LogTemp,Error,TEXT("%s: %s"), *CUR_ACTOR_CLASS_FUNC_LINE, *FString(Text)) 

#define LOG_FLOAT(Text, Float) 	UE_LOG(LogTemp, Warning,TEXT("%s: %s %f"), *CUR_CLASS_FUNC_LINE, *FString(Text), Float)

#define LOG_MULTI(FormatString , ...) UE_LOG(LogTemp, Warning,TEXT("%s: %s"), *CUR_CLASS_FUNC_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )

