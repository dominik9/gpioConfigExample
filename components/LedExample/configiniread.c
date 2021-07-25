#include "configiniread.h"
#include "esp_log.h"


static const char * CONFIG_INI_TAG_LOG = "INI_LOG";

int32_t convertCharToInt(char value){
    return value - '0';
}

void getConfigTag(char * line_buffer, char * tag, char * val){
  //Rozdzielic tag od wartości
  bool tag_in_line = true;
  uint8_t i = 0, tag_val_iter = 0;

  while (line_buffer[i] - '\0'){
    if(tag_in_line == true){
      if(line_buffer[i] == '='){
        tag[tag_val_iter] = '\0';
        tag_in_line = false;
        tag_val_iter = 0;
      }else{
        tag[tag_val_iter] = line_buffer[i];
        tag_val_iter++;
      }
    }else{
      val[tag_val_iter] = line_buffer[i];
      tag_val_iter++;
    }
    i++;
  }
  val[tag_val_iter] = '\0';
}

Config_ini_error_t getConfigFromFile(char * configTag, char * value){
  extern const unsigned char configString[] asm(CONFIG_FILE_START);
  //printf("%s\n", configString);
  int i = 0;
  char line_buffer[50] = "\0";
  char lineTag[40] = "\0";
  int buff_iter = 0;
  while (configString[i] - '\0'){
    line_buffer[buff_iter] = configString[i];
    buff_iter++;
    if (configString[i] == '\n'){
      line_buffer[buff_iter-1] = '\0';
      //printf("Buffer line: %s\n", line_buffer);
      getConfigTag(line_buffer, lineTag, value);
      //printf("Tag = %s, Val = %s\n", lineTag, value);
      if (!strcmp(configTag, lineTag)){ //Czy to jest ten config
        return CONFIG_OK;
      }    
      buff_iter = 0;
    }
    i++;
  }
  return CONFIG_ERROR_CONFIG_TAG;
}

Config_ini_error_t getIntConfigFromFile(char * configTag, int32_t * value){
    char value_buff[VALUE_BUFFER_SIZE];
    Config_ini_error_t error = getConfigFromFile(configTag, value_buff);
    if(error){ 
        configIniErrorDisplay(error); 
        return error;       
    }
    int buff_iter = 0;
    //printf("*value przed zmianami = %d\n", *value);
    *value = 0;
    //printf("value_buff = %s\n",value_buff);
    while(value_buff[buff_iter] != '\0'){
      *value = (*value * 10) + convertCharToInt(value_buff[buff_iter]);
      //printf("*value petla = %d, iter = %d\n", *value, buff_iter);
      buff_iter++;
    }
    return CONFIG_OK;
}

Config_ini_error_t getBoolConfigFromFile(char * configTag, bool * value){
  char value_buff[VALUE_BUFFER_SIZE];
  Config_ini_error_t error = getConfigFromFile(configTag, value_buff);
  if(error){ 
    configIniErrorDisplay(error); 
    return error;       
  }
  int buff_iter = 0;
  if(value_buff[0] == '0'){
    *value = false;
  }else if(value_buff[0] == '1'){
    *value = true;
  }else{
    if (!strcmp("true", value_buff)){
      *value = true;
    }else if(!strcmp("false", value_buff)){
      *value = false;
    }else{
      configIniErrorDisplay(CONFIG_ERROR_BOOL_VALUE);
      return CONFIG_ERROR_BOOL_VALUE;
    }

  }
  
  return CONFIG_OK;
}

Config_ini_error_t getListConfigFromFile(char * configTag, int32_t * value){
  return CONFIG_OK;
}

void configIniErrorDisplay(Config_ini_error_t error){
  switch(error){
    case CONFIG_OK:
      ESP_LOGI(CONFIG_INI_TAG_LOG, "OK");
      break;
    case CONFIG_ERROR_FILE_INI:
      ESP_LOGW(CONFIG_INI_TAG_LOG, "Problem with config file.");
      break;
    case CONFIG_ERROR_CONFIG_TAG:
      ESP_LOGW(CONFIG_INI_TAG_LOG, "Tag does't exist.");
      break;
    case CONFIG_ERROR_BOOL_VALUE:
      ESP_LOGW(CONFIG_INI_TAG_LOG, "Bool value incorrect, check config.ini file.");
      break;
  }
}
