#include "zcommon.acs"


//
//
//Init Functions
//
//



function void ECInvInit(void) //Resets Inventory
{

  TakeInventory("TimePoint",9999);
  GiveInventory("TimePoint",GetCVar("ec_maxtimepoints"));

}


function void ECRecursiveScriptCall(void)
{

  ACS_NamedExecuteAlways("GiveTimePointRecursive",0,0);
  ACS_NamedExecuteAlways("TimePointDisplayRecursive",0,0);

}



//
//
//EternalClockMechanism Functions
//
//



function void ECActivate(void)
{
  
  if (GetCVar("ec_enabletint"))
  {
  	FadeTo (255, 255, 0, 0.1, 0.0);
  }

  GiveInventory("TimeStop",1);
  GiveInventory("TimeIsStopped",1);

}



function void ECDeactivate(void)
{

  if (GetCVar("ec_enabletint"))
  {
  	FadeTo (255, 255, 0, 0.0, 0.0);
  }

  TakeInventory("PowerTimeFreeze",1);
  TakeInventory("TimeIsStopped",1);

}



function void ECActivateSound(void)
{

  if (GetCVar("ec_enablesound"))
  	{
      StopSound(0,CHAN_6);
      StopSound(0,CHAN_7);
      PlaySound(0,"eclock1",6|CHAN_NOPAUSE,1.0,FALSE);
      PlaySound(0,"ticking",7|CHAN_NOPAUSE,1.0,TRUE);
    }

}



function void ECDeactivateSound(void)
{

  if (GetCVar("ec_enablesound"))
  	{
      StopSound(0,CHAN_6);
      StopSound(0,CHAN_7);
      PlaySound(0,"eclock0",6|CHAN_NOPAUSE,1.0,FALSE);
    }
  
}



//
//
//GiveTimePointRecursive Functions
//
//



function void ECRec_Charging(void)
{

  GiveInventory("TimePoint",1);

}


function void ECRec_Draining(void)
{

    if (CheckInventory("TimePoint") > 0)
    {
    	TakeInventory("TimePoint",GetCVar("ec_drainamount"));
        
    }

	else if (CheckInventory("TimePoint") <= 0)
	{
		ECDeactivate();	
		ECDeactivateSound();
	}


}



//
//
//TimepointDisplayRecursive-Hudmessage Functions
//
//



function void ECHudMessageTimePoints(void)
{

  SetFont("BIGFONT");
  HudMessage(d:CheckInventory("timepoint")+1; HUDMSG_PLAIN, 2000000000, CR_BRICK, 0.5, 0.2, 2.0); 

}


//
//
//Scripts
//
//


Script "ECInit" ENTER
{

  ECInvInit();
  ECRecursiveScriptCall();

}

//
//Delays
//

Script "GiveTimePointRecursive" (void)
{

	if (CheckInventory("TimeIsStopped") == 0 && CheckInventory("TimePoint") < GetCVar("ec_maxtimepoints"))
    {
       ECRec_Charging();
       for (int i = 0; i < GetCVar("ec_rechargedelay"); ++i)
        { 
	      Delay(1);
        }
    }


    if (CheckInventory("TimeIsStopped") == 1)
    {
       ECRec_Draining();
       for (int i = 0; i < GetCVar("ec_draindelay"); ++i)
        {
  	     Delay(1);
        }
    	
    }

    Delay(1);

    ACS_NamedExecuteAlways("GiveTimePointRecursive",0,0);

}



Script "EternalClockMechanism" (void)
{


  if (CheckInventory("TimeIsStopped") == 0)
  {

  	ECActivate();
    ECActivateSound();

  	terminate;

  }


  else if (CheckInventory("TimeIsStopped") == 1)
  {

    ECDeactivate();
    ECDeactivateSound();

  	terminate;

  }


}


Script "TimePointDisplayRecursive" (void)
{

  if (CheckInventory("TimePoint") < GetCVar("ec_maxtimepoints"))
   {
     EChudmessageTimePoints();
   }

   Delay(1);

   ACS_NamedExecuteAlways("TimePointDisplayRecursive",0,0);
}