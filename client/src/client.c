#include "client.h"
#include <commons/log.h>
#include <commons/collections/list.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Obtener el valor de la clave "CLAVE" del archivo de configuración
    valor = config_get_string_value(config, "CLAVE");

	// Validar que la clave exista en el archivo de configuración
    if (valor == NULL) {
        log_error(logger, "No se encontró la clave 'CLAVE' en el archivo de configuración");
        abort(); // Terminar el programa si no se encuentra la clave
    }

	// Loguear el valor obtenido
    log_info(logger, "El valor de la clave CLAVE es: %s", valor);

	// Usando el config creado previamente, leemos los valores del config y los 
	
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	// Logueamos el valor de ip y puerto
	log_info(logger, "IP: %s", ip);
	log_info(logger, "Puerto: %s", puerto);

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto, logger);

	fprintf("Conectado al servidor %d\n",conexion);

	// Enviamos al servidor el valor de CLAVE como mensaje
	log_info(logger, "Enviando CLAVE al servidor...");
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	liberar_conexion(conexion);
	
	close(cliente_fd);
	close(server_fd);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	 // Creamos el logger con las configuraciones indicadas
    nuevo_logger = log_create("tp0.log", "Client", true, LOG_LEVEL_INFO);

    // Validamos que el logger se haya creado correctamente
    if (nuevo_logger == NULL) {
        printf("No se pudo crear el logger\n");
        exit(1);
    }	

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	// Crear el config a partir del archivo "cliente.config"
    nuevo_config = config_create("cliente.config");

    // Validar que el config se haya creado correctamente
    if (nuevo_config == NULL) {
        printf("No se pudo leer el archivo de configuración\n");
        abort(); // Terminar el programa si no se puede cargar el config
    }

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// Continuar leyendo y logueando hasta que se ingrese una línea vacía
    while (strcmp(leido, "") != 0) {
        // Loguear la línea ingresada
        log_info(logger, "Se ingresó: %s", leido);

        // Liberar la memoria de la línea leída
        free(leido);

        // Leer la siguiente línea
        leido = readline("> ");
    }

    // Liberar la memoria de la última línea (vacía)
    free(leido);


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	 while (1) {
        leido = readline("> ");
        if (strcmp(leido, "") == 0) {
            free(leido);
            break;
        }
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
    }

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	// Cerramos el logger
    log_destroy(logger);

    // Liberar el config
    config_destroy(config);

    // Liberamos otros recursos (conexion y config) según corresponda
}
