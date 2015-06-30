char *textFileRead( char *fn) {  //read the shader code
	FILE *fp;  
	char *content = NULL;  
	int count=0;  

	if (fn != NULL) {  
		fopen_s(&fp , fn, "rt");

		if (fp != NULL) {  
			fseek(fp, 0, SEEK_END);  
			count = ftell(fp);  
			rewind(fp);  

			if (count > 0) {  
				content = (char *)malloc(sizeof(char) * (count+1));  
				count = fread(content,sizeof(char),count,fp);  
				content[count] = '\0';  
			}  
			fclose(fp);  
		} 
		else 
			std::cout << "Fail to open the shader file" << std::endl;
	}  
	return content;  
} 