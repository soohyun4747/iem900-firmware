#include "usb_out.h"
#include "data_io.h"


#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
#define EXIT_PROGRES    2
#define EXIT_NOWORK     3

#define Data_comp  	 0
#define Data_run     1



extern uint8_t time_year, time_week, time_month, time_date, time_hour, time_min, time_sec;
extern uint8_t	out_usb_mode[6];
uint8_t make_year, make_week, make_month,make_date,make_hour,make_min,make_sec;
uint8_t Tempsett_flag,Voc_change_flag,Main_log_flag,Alarmout_flag,Temp_log_flag;

uint8_t usb_out_flag = 0;

uint16_t Tempsett_NO=0,Voc_change_NO=0,Main_log_NO=0,Alarmout_NO=0,Temp_log_NO=0;


uint8_t usb_out_status = 0;

uint8_t device_name[4];




//========================================================================================================================
//========================================================================================================================
void CH376_Initial(void)
{
	mInitCH376Host( );
}


UINT8	SetFileCreateTime( PUINT8 iFileName, UINT16 iCreateDate, UINT16 iCreateTime )
{
	UINT8	s;
	UINT8	buf[64];

	s = CH376FileOpen( iFileName );
	if ( s == USB_INT_SUCCESS ) {

		s = CH376DirInfoRead( );
		if ( s == USB_INT_SUCCESS ) {

			buf[0] = (UINT8)iCreateTime;
			buf[1] = (UINT8)(iCreateTime>>8);
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtTime ), 2 );
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_WrtTime ), 2 ); 
			buf[0] = (UINT8)iCreateDate;
			buf[1] = (UINT8)(iCreateDate>>8);
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtDate ), 2 );
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_WrtDate ), 2 );
			s = CH376DirInfoSave( );
			if ( s == USB_INT_SUCCESS ) {
			}
		}
		CH376FileClose( FALSE );
	}
	return( s );
}


uint8_t Memory_check(void)
{
uint8_t i; 

	for(i=0;i<4;i++)
	{
		if (CH376DiskConnect( ) == USB_INT_SUCCESS )
		{ 
			if(CH376DiskMount( )== USB_INT_SUCCESS)
			{
				if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED) 
				{
					return EXIT_SUCCESS;
				}
			}
		}
	}

	return EXIT_FAILURE;
}


u32 digit(u8 n)	//10^n
{
u32 result=1;	 

	while(n--)	result*=10;
	return result;
}			 



void Copy_data(uint32_t data, uint8_t *copy_adr, uint8_t start_adr, uint8_t count)
{
uint8_t	i,temp;

	copy_adr += start_adr;

	for(i=0;i<count;i++)
	{
		temp=(data/digit(count-i-1))%10;
		*(copy_adr++) = temp +'0';
	}
}



void Copy_data2(uint32_t data, uint8_t *copy_adr, uint8_t start_adr)
{
uint8_t	temp;

	copy_adr += start_adr;

	temp=(data/100)%10;
	*(copy_adr++) = temp +'0';

	temp=(data/10)%10;
	*(copy_adr++) = temp +'0';

	*(copy_adr++) = '.';

	temp=(data)%10;
	*(copy_adr++) = temp +'0';

}


void device_name_start(uint8_t *copy_adr)
{
	copy_adr[0] = device_name[0];
	copy_adr[1] = device_name[1];
	copy_adr[2] = device_name[2];
	copy_adr[3] = device_name[3];
	copy_adr[4] = '_';
	copy_adr[5] = 0;
}



uint8_t file_make(uint16_t *file_no, uint8_t *TarName, uint8_t *SrcName)
{
	UINT8	s;
	UINT16	j;
	char	buf[70];

	if(*file_no==9999)	*file_no=0;

	for(j=*file_no;j<=9999;j++)
	{
		Copy_data( j, (uint8_t *)TarName, 5, 4);

		s=CH376FileOpen((uint8_t *)TarName); 
		
		if( s == ERR_MISS_FILE )
		{
			break;
		}
	}

	(*file_no) = ++j;

	Copy_data( make_year,  (uint8_t *)SrcName, 16, 2);
	Copy_data( make_month, (uint8_t *)SrcName, 18, 2);
	Copy_data( make_date,  (uint8_t *)SrcName, 20, 2);
	Copy_data( make_hour,  (uint8_t *)SrcName, 23, 2);
	Copy_data( make_min,   (uint8_t *)SrcName, 25, 2);
	Copy_data( make_sec,   (uint8_t *)SrcName, 27, 2);

	for ( j = 0; SrcName[j] != 0; j++ ) 
	{ 
		buf[j*2] = SrcName[j];
		buf[j*2+1] = 0x00;
	}
	buf[j*2] = 0x00;  
	buf[j*2+1] = 0x00;

	s = CH376CreateLongName( (uint8_t *)TarName, (uint8_t *)buf ); 
	if (s == EXIT_FAILURE)  return EXIT_FAILURE;

	s = SetFileCreateTime( (uint8_t *)TarName , MAKE_FILE_DATE( 2000+make_year, make_month, make_date ), MAKE_FILE_TIME( make_hour, make_min, make_sec ) ); 
	if (s == USB_INT_SUCCESS)  return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

//========================================================================================================================
//========================================================================================================================
uint8_t Tempsett_data(uint16_t data_no, uint8_t *mode_data,  uint8_t *cham_data, uint16_t *set_data, uint8_t *time_data, char *out_data )
{
uint16_t line,dat,flag;
char	buf[50];
uint8_t temp;

	out_data[0] = 0 ;	//null

	dat = data_no*10+1;

	for(line=0;line<10;line++)		//44*10=440切
	{
		flag = *mode_data++;
		if (flag >= 5) return Data_comp;
	
    strcpy( buf, "0000,00/00/00 00:00:00,        ,00.0\r\n" );

		Copy_data( dat+line, (uint8_t *)buf, 0, 4);			//no

		Copy_data( *time_data++, (uint8_t *)buf,  5, 2);	//time_data
		Copy_data( *time_data++, (uint8_t *)buf,  8, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 11, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 14, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 17, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 20, 2);

 		temp = *cham_data++;
		buf[23] = (temp & 0x01)?  '1' : ' ' ;
		buf[24] = (temp & 0x02)?  '2' : ' ' ;
		buf[25] = (temp & 0x04)?  '3' : ' ' ;
		buf[26] = (temp & 0x08)?  '4' : ' ' ;
		buf[27] = (temp & 0x10)?  '5' : ' ' ;
		buf[28] = (temp & 0x20)?  '6' : ' ' ;
		buf[29] = (temp & 0x40)?  '7' : ' ' ;
		buf[30] = (temp & 0x80)?  '8' : ' ' ;

    switch(flag)
    {
      case 0:
        Copy_data2( *set_data++, (uint8_t *)buf, 32);	//
        break;
      case 1:
        Copy_data2( *set_data++, (uint8_t *)buf, 32);	//
        strncpy( buf+36, "T cal ", 6);
        break;
      case 2:
        Copy_data2( *set_data++, (uint8_t *)buf, 32);	//
        strncpy( buf+36, "B cal ", 6);
        break;
      case 3:
        set_data++;
        strncpy( buf+32, "T Init", 6);
        break; 
      case 4:
        set_data++;
        strncpy( buf+32, "B Init", 6);
        break;      
    }

		strcat( out_data, buf );
	}
	return 	Data_run;
}



uint8_t Tempsett_output(void)
{
	UINT8	i, s, t;
	char	TarName[16];
	char	SrcName[40];
	char	buf[512];
	static uint8_t mode_cnt = 0;
	static uint16_t data_cnt = 0;

	uint8_t mode_data[10];
	uint8_t time_data[60];
	uint8_t cham_data[10];
	uint16_t set_data[10];


	switch(mode_cnt)
	{
		case 0:	
			strcpy( TarName, "/Tpst0000.CSV" );
			device_name_start( (uint8_t *)SrcName);
			strcat( SrcName, "Tempsett_20000000_000000.CSV" );

			s = Memory_check();
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;
	
			s = file_make( &Tempsett_NO,(uint8_t *)TarName, (uint8_t *)SrcName);
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = CH376FileOpen((uint8_t *)TarName); 
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			strcpy( buf, "NO.,Date/Time,Chamber,Set\r\n" );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 1;
			data_cnt = 0;
			break;

		case 1:		
			for(i = 0; i < 10; i++)
			{
        Temp_set_Read(data_cnt*10, 10, mode_data, cham_data, set_data, time_data);
        
				t = Tempsett_data(data_cnt, mode_data,  cham_data, set_data, time_data, (char *)buf);		//0000,00/00/00 00:00:00,        ,00.0\r\n  38切
				s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
				if (t == Data_comp || ++data_cnt == 500)	{mode_cnt = 2;	break;}
			}
			break;

		case 2:	
			mode_cnt = 0;
			s = CH376FileClose( TRUE );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 3;
			break;

		case 3:	
			mode_cnt = 0;
			return EXIT_SUCCESS;
	}

	return	EXIT_PROGRES;
}



uint8_t Tempsett_file_routin(void)
{
uint8_t sta;

	if (Tempsett_flag == 1)
	{

		sta = Tempsett_output();

		if(sta == EXIT_SUCCESS)	
		{
			Tempsett_flag = 0;
			return	EXIT_SUCCESS;
		}
		else if (sta == EXIT_FAILURE)	
		{
			Tempsett_flag = 0;
			return	EXIT_FAILURE;
		}
		else if (sta == EXIT_PROGRES)	
		{
			return	EXIT_PROGRES;
		}
	}

	return	EXIT_NOWORK;

}


//========================================================================================================================
//========================================================================================================================
uint8_t Voc_change_data(uint16_t data_no, uint8_t *dis_data, char *out_data )
{
uint16_t line,dat;
char	buf[30];
uint8_t temp;

	out_data[0] = 0 ;	//null

	dat = data_no*10+1;

	for(line=0;line<10;line++)		//29*10=290切
	{
		temp = *dis_data++ ;
		if (temp != 1) return Data_comp;

		strcpy( buf, "00,00/00/00 00:00:00,0Month\r\n" );

		Copy_data( dat+line, (uint8_t *)buf, 0, 2);			//no

		Copy_data( *dis_data++, (uint8_t *)buf,  3, 2);	//time_data
		Copy_data( *dis_data++, (uint8_t *)buf,  6, 2);
		Copy_data( *dis_data++, (uint8_t *)buf,  9, 2);
		Copy_data( *dis_data++, (uint8_t *)buf, 12, 2);
		Copy_data( *dis_data++, (uint8_t *)buf, 15, 2);
		Copy_data( *dis_data++, (uint8_t *)buf, 18, 2);

		Copy_data( *dis_data++, (uint8_t *)buf, 21, 1);

		strcat( out_data, buf );
	}
	return 	Data_run;
}


uint8_t Voc_change_output(void)
{
	UINT8	i, s, t;
	char	TarName[16];
	char	SrcName[40];
	char	buf[512];
	static uint8_t mode_cnt = 0;
	static uint16_t data_cnt = 0;

	uint8_t dis_data[80];

	switch(mode_cnt)
	{
		case 0:	
			strcpy( TarName, "/Voch0000.CSV" );
			device_name_start( (uint8_t *)SrcName);      
			strcat( SrcName, "Vocchang_20000000_000000.CSV" );

			s = Memory_check();
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = file_make( &Voc_change_NO,(uint8_t *)TarName, (uint8_t *)SrcName);
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = CH376FileOpen((uint8_t *)TarName); 
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			strcpy( buf, "NO.,Date/Time,Valid time\r\n" );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 1;
			data_cnt = 0;
			break;

		case 1:		
			for(i = 0; i < 10; i++)
			{
        Voc_change_Read(data_cnt*10, 10, dis_data);
				t = Voc_change_data(data_cnt, dis_data, (char *)buf);		//00,00/00/00 00:00:00,0Month\r\n  29切
				s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
				if (t == Data_comp || ++data_cnt == 5)	{mode_cnt = 2;	break;}
			}
			break;

		case 2:	
			mode_cnt = 0;
			s = CH376FileClose( TRUE );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 3;
			break;

		case 3:	
			mode_cnt = 0;
			return EXIT_SUCCESS;
	}

	return	EXIT_PROGRES;
}



uint8_t Voc_change_routin(void)
{
uint8_t sta;

	if (Voc_change_flag == 1)
	{
		sta = Voc_change_output();

		if(sta == EXIT_SUCCESS)	
		{
			Voc_change_flag = 0;
			return	EXIT_SUCCESS;
		}
		else if (sta == EXIT_FAILURE)	
		{
			Voc_change_flag = 0;
			return	EXIT_FAILURE;
		}
		else if (sta == EXIT_PROGRES)	
		{
			return	EXIT_PROGRES;
		}
	}

	return	EXIT_NOWORK;

}

//========================================================================================================================
//========================================================================================================================
uint8_t Main_log_data(uint16_t data_no, uint8_t *dis_data1, uint8_t *dis_data2,  char *out_data )
{
uint16_t line,dat;
char	buf[50];
uint8_t temp0,temp1,temp2,temp3;

	out_data[0] = 0 ;	//null

	dat = data_no*10+1;

	for(line=0;line<10;line++)		//49*10=490切
	{
		temp0 = *dis_data1++;
		if (temp0 != 1) return Data_comp;

		strcpy( buf, "0000,00/00/00 00:00:00,           ,            \r\n" );

		Copy_data( dat+line, (uint8_t *)buf, 0, 4);			//no

		Copy_data( *dis_data2++, (uint8_t *)buf,  5, 2);	//time_data
		Copy_data( *dis_data2++, (uint8_t *)buf,  8, 2);
		Copy_data( *dis_data2++, (uint8_t *)buf, 11, 2);
		Copy_data( *dis_data2++, (uint8_t *)buf, 14, 2);
		Copy_data( *dis_data2++, (uint8_t *)buf, 17, 2);
		Copy_data( *dis_data2++, (uint8_t *)buf, 20, 2);

		temp1 = *dis_data1++;
		temp2 = *dis_data1++;
		temp3 = *dis_data1++;
		
		switch(temp1)
		{
			case 1: 
				strncpy( buf+23, "Temperature", 11);
				strncpy( buf+35, "Set Change  ", 12);

				break;
			case 2: 
				strncpy( buf+23, "Chamber No%", 11);

				Copy_data( temp2+1, (uint8_t *)buf, 33, 1);

				if (temp3 ==0x00)
				{	strncpy( buf+35, "Door Open   ", 12);}
				else
				{	strncpy( buf+35, "Door Close  ", 12);}
				break;

			case 3: 
				strncpy( buf+23, "Chamber No%", 11);
				Copy_data( temp2+1, (uint8_t *)buf, 33, 1);

				if (temp3 ==0x00)
				{	strncpy( buf+35, "CultureEmpty", 12);}
				else
				{	strncpy( buf+35, "CultureStart", 12);}
				break;

			case 4: 
				strncpy( buf+23, "VOC Filter ", 11);
				strncpy( buf+35, "%Month Start", 12);
				Copy_data( temp2, (uint8_t *)buf, 35, 1);
				break;

			case 5: 
				strncpy( buf+23, "Alarm List ", 11);

				switch(temp3)
				{
					case 1: 
						strncpy( buf+35, "Door Open   ", 12);
						break;
					case 2: 
					case 3: 
						strncpy( buf+35, "Temper High ", 12);
						break;
					case 4: 
					case 5: 
						strncpy( buf+35, "Temper Low  ", 12);
						break;				
					case 6: 
					case 7: 
						strncpy( buf+35, "Sensor Error", 12);
						break;	
					case 8: 
						strncpy( buf+35, "Gas Pressure", 12);
						break;			
					case 9: 
						strncpy( buf+35, "Valve Error ", 12);
						break;				
					case 10: 
						strncpy( buf+35, "VOC Change  ", 12);
						break;	
				}		
				break;

			case 6: 
				strncpy( buf+23, "Mode Change", 11);
				if (temp3 ==0x00)
				{	strncpy( buf+35, "OIL Mode    ", 12);}
				else
				{	strncpy( buf+35, "WET Mode    ", 12);}
				break;

			case 7: 
				strncpy( buf+23, "CO2 Change ", 11);
				strncpy( buf+35, "%.% Percent ", 12);
				Copy_data( temp2/10, (uint8_t *)buf, 35, 1);
				Copy_data( temp2%10, (uint8_t *)buf, 37, 1);
				break;

			case 8: 
				strncpy( buf+23, "O2 Change  ", 11);
				strncpy( buf+35, "%.% Percent ", 12);
				Copy_data( temp2/10, (uint8_t *)buf, 35, 1);
				Copy_data( temp2%10, (uint8_t *)buf, 37, 1);
				break;

			case 9: 
				strncpy( buf+23, "Calibration", 11);
				strncpy( buf+35, "Chamber No% ", 12);
        Copy_data( temp2+1, (uint8_t *)buf, 45, 1);
				break;

			case 10: 
				strncpy( buf+23, "Initialize ", 11);
				strncpy( buf+35, "Chamber No% ", 12);
        Copy_data( temp2+1, (uint8_t *)buf, 45, 1);
				break;

		}
		strcat( out_data, buf );
	}
	return 	Data_run;
}


uint8_t Main_log_output(void)
{
	UINT8	i, s, t;
	char	TarName[16];
	char	SrcName[40];
	char	buf[512];
	static uint8_t mode_cnt = 0;
	static uint16_t data_cnt = 0;

	uint8_t dis_data1[40];
	uint8_t dis_data2[60];
	
	switch(mode_cnt)
	{
		case 0:	
			strcpy( TarName, "/Malg0000.CSV" );
			device_name_start( (uint8_t *)SrcName);      
			strcat( SrcName, "Main_log_20000000_000000.CSV" );

			s = Memory_check();
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = file_make( &Main_log_NO ,(uint8_t *)TarName, (uint8_t *)SrcName);
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = CH376FileOpen((uint8_t *)TarName); 
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			strcpy( buf, "NO.,Date/Time,Item,Signal\r\n" );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 1;
			data_cnt = 0;
			break;

		case 1:	
			for(i = 0; i < 10; i++)
			{
        Main_log_Read(data_cnt*10, 10, dis_data1, dis_data2);
				t = Main_log_data(data_cnt, dis_data1, dis_data2,  (char *)buf);		//0000,00/00/00 00:00:00,           ,            \r\n ;49切
				s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
				if (t == Data_comp || ++data_cnt == 500)	{mode_cnt = 2;	break;}
			}
			break;

		case 2:	
			mode_cnt = 0;
			s = CH376FileClose( TRUE );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 3;
			break;

		case 3:	
			mode_cnt = 0;
			return EXIT_SUCCESS;
	}

	return	EXIT_PROGRES;
}


uint8_t Main_log_file_routin(void)
{
uint8_t sta;

	if (Main_log_flag == 1)
	{
		sta = Main_log_output();

		if(sta == EXIT_SUCCESS)	
		{
			Main_log_flag = 0;
			return	EXIT_SUCCESS;
		}
		else if (sta == EXIT_FAILURE)	
		{
			Main_log_flag = 0;
			return	EXIT_FAILURE;
		}
		else if (sta == EXIT_PROGRES)	
		{
			return	EXIT_PROGRES;
		}
	}

	return	EXIT_NOWORK;

}

//========================================================================================================================
//========================================================================================================================
uint8_t Alarmout_data(uint16_t data_no, uint8_t *alarm_num, uint8_t *alarm_ont, uint8_t *alarm_off,  char *out_data )
{
uint16_t line,dat,flag;
char	buf[70];
uint8_t sub_err, err_d;

	out_data[0] = 0 ;	//null

	dat = data_no*6+1;

	for(line=0;line<6;line++)		//67*6=402
	{
		flag = *alarm_num++;
		err_d = *alarm_num++;
		sub_err = *alarm_num++;

		if (flag != 1) return Data_comp;

		strcpy( buf, "00,00/00/00 00:00:00,00/00/00 00:00:00,                          \r\n" );

		Copy_data( dat+line, (uint8_t *)buf, 0, 2);			//no

		Copy_data( *alarm_ont++, (uint8_t *)buf,  3, 2);	//time_data
		Copy_data( *alarm_ont++, (uint8_t *)buf,  6, 2);
		Copy_data( *alarm_ont++, (uint8_t *)buf,  9, 2);
		Copy_data( *alarm_ont++, (uint8_t *)buf, 12, 2);
		Copy_data( *alarm_ont++, (uint8_t *)buf, 15, 2);
		Copy_data( *alarm_ont++, (uint8_t *)buf, 18, 2);

		Copy_data( *alarm_off++, (uint8_t *)buf, 21, 2);	//time_data
		Copy_data( *alarm_off++, (uint8_t *)buf, 24, 2);
		Copy_data( *alarm_off++, (uint8_t *)buf, 27, 2);
		Copy_data( *alarm_off++, (uint8_t *)buf, 30, 2);
		Copy_data( *alarm_off++, (uint8_t *)buf, 33, 2);
		Copy_data( *alarm_off++, (uint8_t *)buf, 36, 2);


		switch(err_d)
		{
			case 1 : 
					strncpy( buf+39, "CHAMBER% DOOR OPEN!       ", 26);		//EEROR 1
					Copy_data( sub_err+1, (uint8_t *)buf, 46, 1);
					break;
			case 2 : 
					strncpy( buf+39, "CHAMBER% TOP-HEATER HI ERR", 26);	//EEROR 2
					Copy_data( sub_err+1, (uint8_t *)buf, 46, 1);
					break;
			case 3 : 
					strncpy( buf+39, "CHAMBER% BOT-HEATER HI ERR", 26);	//EEROR 3
					Copy_data( sub_err+1, (uint8_t *)buf, 46, 1);
					break;
			case 4 : 
					strncpy( buf+39, "CHAMBER% TOP-HEATER LO ERR", 26);	//EEROR 4
					Copy_data( sub_err+1, (uint8_t *)buf, 46, 1);
					break;
			case 5 : 
					strncpy( buf+39, "CHAMBER% BOT-HEATER LO ERR", 26);	//EEROR 5
					Copy_data( sub_err+1, (uint8_t *)buf, 46, 1);
					break;	
			case 6 : 
					strncpy( buf+39, "CHAMBER% TOPTMP SENSOR ERR", 26);	//EEROR 6
					Copy_data( sub_err+1, (uint8_t *)buf, 46, 1);
					break;
			case 7 : 
					strncpy( buf+39, "CHAMBER% BOTTMP SENSOR ERR", 26);	//EEROR 7
					Copy_data( sub_err+1, (uint8_t *)buf, 46, 1);
					break;
			case 8 : 
					strncpy( buf+39, "GAS SUPPLY PRESSURE ERROR ", 26);	//EEROR 8
					break;
			case 9 : 
					strncpy( buf+39, "GAS SOL VALVE ERROR       ", 26);	//EEROR 9
					break;
			case 10: 
					strncpy( buf+39, "VOC FILTER REMAIN DAY %%  ", 26);	//EEROR 10
					Copy_data( sub_err+1, (uint8_t *)buf, 61, 2);
					break;	
			case 11: 
					strncpy( buf+39, "POWER OFF STRANGE ERROR   ", 26);	//EEROR 10
					break;
		}
		strcat( out_data, buf );
	}
	return 	Data_run;
}


uint8_t Alarmout_output(void)
{
	UINT8	i, s, t;
	char	TarName[16];
	char	SrcName[40];
	char	buf[512];
	static uint8_t mode_cnt = 0;
	static uint16_t data_cnt = 0;

	uint8_t alarm_num[24];
	uint8_t alarm_ont[72];
	uint8_t alarm_off[72];

	switch(mode_cnt)
	{
		case 0:	
			strcpy( TarName, "/Alam0000.CSV" );
			device_name_start( (uint8_t *)SrcName);        
			strcat( SrcName, "Alarmout_20000000_000000.CSV" );

			s = Memory_check();
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = file_make( &Alarmout_NO,(uint8_t *)TarName, (uint8_t *)SrcName);
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = CH376FileOpen((uint8_t *)TarName); 
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			strcpy( buf, "NO.,Date/Time,Cancle,Alarm\r\n" );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 1;
			data_cnt = 0;
			break;

		case 1:	
			for(i = 0; i < 10; i++)
			{
        Alarm_log_Read(data_cnt*6, 6, alarm_num, alarm_ont, alarm_off);

				t = Alarmout_data(data_cnt, alarm_num, alarm_ont, alarm_off, (char *)buf);					//00,00/00/00 00:00:00,00/00/00 00:00:00,                          \r\n ;67切
				s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
				if (t == Data_comp || ++data_cnt == 10)	{mode_cnt = 2;	break;}
			}
			break;

		case 2:	
			mode_cnt = 0;
			s = CH376FileClose( TRUE );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 3;
			break;

		case 3:	
			mode_cnt = 0;
			return EXIT_SUCCESS;
	}

	return	EXIT_PROGRES;
}


uint8_t Alarmout_file_routin(void)
{
uint8_t sta;

	if (Alarmout_flag == 1)
	{
		sta = Alarmout_output();

		if(sta == EXIT_SUCCESS)	
		{
			Alarmout_flag = 0;
			return	EXIT_SUCCESS;
		}
		else if (sta == EXIT_FAILURE)	
		{
			Alarmout_flag = 0;
			return	EXIT_FAILURE;
		}
		else if (sta == EXIT_PROGRES)	
		{
			return	EXIT_PROGRES;
		}
	}

	return	EXIT_NOWORK;

}

//========================================================================================================================
//========================================================================================================================
uint8_t Temp_log_data(uint16_t data_no, uint8_t *time_data, uint16_t *now_data, uint16_t *set_data,  char *out_data )
{
uint16_t line,dat;
char	buf[108];
uint8_t temp;

	out_data[0] = 0 ;	//null

	dat = data_no*4+1;

	for(line=0;line<4;line++)		//104*4=416切
	{
		temp = *time_data++;

		if (temp != 1) return Data_comp;

		strcpy( buf, "0000,00/00/00 00:00:00,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0\r\n" );	//104

		Copy_data( dat+line, (uint8_t *)buf, 0, 4);			//no

		temp = *time_data++;

		Copy_data( *time_data++, (uint8_t *)buf,  5, 2);	//time_data
		Copy_data( *time_data++, (uint8_t *)buf,  8, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 11, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 14, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 17, 2);
		Copy_data( *time_data++, (uint8_t *)buf, 20, 2);

		Copy_data2( *now_data++, (uint8_t *)buf, 23);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 28);	//

		Copy_data2( *now_data++, (uint8_t *)buf, 33);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 38);	//

		Copy_data2( *now_data++, (uint8_t *)buf, 43);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 48);	//

		Copy_data2( *now_data++, (uint8_t *)buf, 53);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 58);	//

		Copy_data2( *now_data++, (uint8_t *)buf, 63);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 68);	//

		Copy_data2( *now_data++, (uint8_t *)buf, 73);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 78);	//

		Copy_data2( *now_data++, (uint8_t *)buf, 83);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 88);	//

		Copy_data2( *now_data++, (uint8_t *)buf, 93);	//
		Copy_data2( *set_data++, (uint8_t *)buf, 98);	//

		strcat( out_data, buf );
	}
	return 	Data_run;
}


uint8_t Temp_log_output(void)
{
	UINT8	i, s, t;
	char	TarName[16];
	char	SrcName[40];
	char	buf[512];
	static uint8_t mode_cnt = 0;
	static uint16_t data_cnt = 0;

	uint8_t time_data[32];
	uint16_t now_data[32];
	uint16_t set_data[32];

	switch(mode_cnt)
	{
		case 0:	
			strcpy( TarName, "/Tplg0000.CSV" );
			device_name_start( (uint8_t *)SrcName);      
			strcat( SrcName, "Temp_log_20000000_000000.CSV" );

			s = Memory_check();
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = file_make( &Temp_log_NO, (uint8_t *)TarName, (uint8_t *)SrcName);
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			s = CH376FileOpen((uint8_t *)TarName); 
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			strcpy( buf, "NO.,Date/Time,");
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			strcpy( buf, "Ch1 Temp.,Ch1 Sett.,Ch2 Temp.,Ch2 Sett.," );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			strcpy( buf, "Ch3 Temp.,Ch3 Sett.,Ch4 Temp.,Ch4 Sett.," );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			strcpy( buf, "Ch5 Temp.,Ch5 Sett.,Ch6 Temp.,Ch6 Sett.," );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			strcpy( buf, "Ch7 Temp.,Ch7 Sett.,Ch8 Temp.,Ch8 Sett.\r\n" );
			s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 1;
			data_cnt = 0;
			break;

		case 1:	
			for(i = 0; i < 10; i++)
			{
        Cham_temp_Read(data_cnt*4, 4, time_data, now_data, set_data);
							
				t = Temp_log_data(data_cnt, time_data, now_data, set_data, (char *)buf);		//0000,00/00/00 00:00:00,00.0,00.0,00.0,00.0,00.0,00.0,
				s = CH376ByteWrite( (uint8_t *)buf, strlen(buf), NULL );						//00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,00.0,\r\n   ;104切
				if (t == Data_comp || ++data_cnt == 1500)	{mode_cnt = 2;	break;}
			}
			break;

		case 2:	
			mode_cnt = 0;
			s = CH376FileClose( TRUE );
			if (s== EXIT_FAILURE)	return EXIT_FAILURE;

			mode_cnt = 3;
			break;

		case 3:	
			mode_cnt = 0;
			return EXIT_SUCCESS;
	}
	return	EXIT_PROGRES;
}



uint8_t Temp_log_file_routin(void)
{
uint8_t sta;

	if (Temp_log_flag == 1)
	{
		sta = Temp_log_output();

		if(sta == EXIT_SUCCESS)	
		{
			Temp_log_flag = 0;
			return	EXIT_SUCCESS;
		}
		else if (sta == EXIT_FAILURE)	
		{
			Temp_log_flag = 0;
			return	EXIT_FAILURE;
		}
		else if (sta == EXIT_PROGRES)	
		{
			return	EXIT_PROGRES;
		}
	}
	
	return	EXIT_NOWORK;

}


//========================================================================================================================
//========================================================================================================================
void usb_out_data(void)
{
static uint8_t usb_temp=0;
uint8_t sta;
	
	if ( usb_out_flag == 0x01 ) 
	{
		switch(usb_temp)
		{
			case 0: 
				make_year = time_year;
				make_month = time_month;
				make_date = time_date;	
				make_hour = time_hour;
				make_min = time_min;
				make_sec = time_sec;	
		
				Tempsett_flag = 0;
				Voc_change_flag = 0;
				Main_log_flag = 0;
				Alarmout_flag = 0;
				Temp_log_flag = 0;
			
				usb_out_status = 0;		// 0= 舛雌, 1= 20%, 2= 40%, 3= 60%, 4= 80%, 5 = 100%, 6= error
			
				if ( out_usb_mode[0] ) 	Tempsett_flag = 1;
				if ( out_usb_mode[1] ) 	Voc_change_flag = 1;
				if ( out_usb_mode[2] ) 	Main_log_flag = 1;
				if ( out_usb_mode[3] ) 	Alarmout_flag = 1;
				if ( out_usb_mode[4] ) 	Temp_log_flag = 1;

				usb_temp = 1;
				break;
			case 1: 
				sta = Tempsett_file_routin();

				if (sta == EXIT_SUCCESS)			{ usb_temp = 2 ; usb_out_status=1; }
				else if (sta == EXIT_FAILURE)		{ usb_temp = 6 ; usb_out_status=6; }
				else if (sta == EXIT_NOWORK)		{ usb_temp = 2 ; usb_out_status=1; }
				break;
			case 2: 
				sta = Voc_change_routin();
		
				if (sta == EXIT_SUCCESS)			{ usb_temp = 3 ; usb_out_status=2; }
				else if (sta == EXIT_FAILURE)		{ usb_temp = 6 ; usb_out_status=6; }
				else if (sta == EXIT_NOWORK)		{ usb_temp = 3 ; usb_out_status=2; }
				break;	
			case 3: 
				sta = Main_log_file_routin();

				if (sta == EXIT_SUCCESS)			{ usb_temp = 4 ; usb_out_status=3; }
				else if (sta == EXIT_FAILURE)		{ usb_temp = 6 ; usb_out_status=6; }
				else if (sta == EXIT_NOWORK)		{ usb_temp = 4 ; usb_out_status=3; }
				break;	
			case 4: 
				sta = Alarmout_file_routin();

				if (sta == EXIT_SUCCESS)			{ usb_temp = 5 ; usb_out_status=4; }
				else if (sta == EXIT_FAILURE)		{ usb_temp = 6 ; usb_out_status=6; }
				else if (sta == EXIT_NOWORK)		{ usb_temp = 5 ; usb_out_status=4; }
				break;	
			case 5: 
				sta = Temp_log_file_routin();

				if (sta == EXIT_SUCCESS)			{ usb_temp = 6 ; usb_out_status=5; }
				else if (sta == EXIT_FAILURE)		{ usb_temp = 6 ; usb_out_status=6; }
				else if (sta == EXIT_NOWORK)		{ usb_temp = 6 ; usb_out_status=5; }
				break;	
			case 6: 
				usb_temp = 0;
				usb_out_flag = 0;
				break;				
		}
	}

}


