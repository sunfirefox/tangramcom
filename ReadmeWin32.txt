The TangramCOM can work in both Linux and windows..Please visit http://sites.google.com/site/tangramcom/ to get more information.

Note: Please replace following TANGRAMPATH with the tangram source code path,example :D:\Tangram

We have test on windows2003/windowsXP with MVS7.1

----------------------------------------------------------------------------------------------------
1 Build TangramCOM
	The TangramCOM Release Packet include:
		1) libs for DCOM
		2) tgmregsvr32:	COM register tool
		3) widl:	IDL compiler
		4) TalkToStranger:	a  talk demo for out-process COM
 
	For build all on linux ,just open follows solution:
 
		1) TangramCom.sln to compiler DCOM lib
		2) tools\tools.sln to compiler widl.exe and tgmregssvr32.exe
		3) tests\TalkToStranger\TalkToStranger.sln	to compiler test programs
 

and you can found all binary in TANGRAMPATH/bin/win32
 
----------------------------------------------------------------------------------------------------	
2 Register TangramCOM and test programs:
 
		cd TANGRAMPATH/bin/win32
		A_TGMRegisterCOM.bat
 
	then the register infomation should store in  "d:\tangram_registery.xml",and you can modify tgmwine_config.h to change another path
 

----------------------------------------------------------------------------------------------------
3 RunTest
	comchat is a chat room demo ,and is take from <<COM esstential>>
	sure the current path is TANGRAMPATH/bin/win32
 
	1)start scm server
			scm_daemon
	2)start server
			talk_server /AUTOMATION
		and wait until display "Server is Ready!"
	3)start two or more client
			talk_client 
                    Until display "you        :" then input "/start" to start a new talk session with "Stranger"
