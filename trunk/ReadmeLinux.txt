The TangramCOM can work in both Linux and windows.Please visit http://sites.google.com/site/tangramcom/ to get more information.

Note: Please replace following TANGRAMPATH with the tangram source code path,example :/Work/Tangram

----------------------------------------------------------------------------------------------------
1 Build TangramCOM
	The TangoraCOM Release Packet include:
		1) libs for DCOM
		2) tgmregsvr32:	COM register tool
		3) widl:	IDL compiler
		4) TalkToStranger:	a  talk demo for out-process COM
 
	For build all on Linux ,just cd the TANGRAMPATH and command:
 
		./Buildit

and you can found all binary in TANGRAMPATH/bin/linux
 
----------------------------------------------------------------------------------------------------	
2 Register TangramCOM and test programs:
 
		export LD_LIBRARY_PATH=TANGRAMPATH/bin/linux:$LD_LIBRARY_PATH
		cd TANGRAMPATH/bin/linux
		chmod +x A_TGMRegisterCOM
		./A_TGMRegisterCOM
 
	then the register infomation should store in  "./tangram_registery.xml",and you can modify tgmwine_config.h to change the path.
 
----------------------------------------------------------------------------------------------------
3 Run tests
	TalkToStranger is a simple demo for COM Server ,and run like OMEGLE
	please sure TANGRAMPATH/bin/linux is in LD_LIBRARY_PATH
 
        	1)start scm server
            	 ./scm_daemon
        
        2)start server
            	 ./talk_server /AUTOMATION
        	 and wait until display "Server is Ready!"

        3)start two or more client
            	 ./talk_client 
           Until display "you        :" then input "/start" to start a new talk session with "Stranger"

----------------------------------------------------------------------------------------------------			
4 More...
	We have test on SUSE9.3,Open Suse10.2.
	
		

	

	
 
