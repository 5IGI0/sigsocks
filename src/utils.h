#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * @brief convert sockaddr to a string
 * 
 * @param sa 
 * @return char* ip
 */
char *get_ip_str(const struct sockaddr *sa);

#endif