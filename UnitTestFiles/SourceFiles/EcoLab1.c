/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include <time.h>

#include "IEcoCalculatorY.h"
#include "IEcoCalculatorX.h"

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t ECOCALLMETHOD EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;

    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

	{
		IEcoCalculatorX* pICalculatorX = 0;
		IEcoCalculatorY* pICalculatorY = 0;
		int32_t operation_result = 0;

		printf("Operations test:\n");
		if (pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pICalculatorX) == 0 && pICalculatorX != 0) {
			operation_result = pICalculatorX->pVTbl->Addition(pICalculatorX, 10, 5);
			printf("    IEcoCalculatorX: 10 + 5 = %d (aggregation)\n", operation_result);
			operation_result = pICalculatorX->pVTbl->Subtraction(pICalculatorX, 10, 5);
			printf("    IEcoCalculatorX: 10 - 5 = %d (aggregation)\n", operation_result);
			pICalculatorX->pVTbl->Release(pICalculatorX);
			pICalculatorX = 0;
        }
		if (pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pICalculatorY) == 0 && pICalculatorY != 0) {
			operation_result = pICalculatorY->pVTbl->Multiplication(pICalculatorY, 10, 5);
			printf("    IEcoCalculatorY: 10 * 5 = %d (inclusion)\n", operation_result);
			operation_result = pICalculatorY->pVTbl->Division(pICalculatorY, 10, 5);
			printf("    IEcoCalculatorY: 10 / 5 = %d (inclusion)\n", operation_result);
			pICalculatorY->pVTbl->Release(pICalculatorY);
			pICalculatorY = 0;
		}

        printf("Test interface accessibility:\n");
		result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pICalculatorX);
        if (result == 0) {
            pICalculatorX->pVTbl->Release(pICalculatorX);
            printf("    IEcoLab1 -> ICalculatorX: Success\n");
        }
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pICalculatorY);
        if (result == 0) {
            pICalculatorY->pVTbl->Release(pICalculatorY);
            printf("    IEcoLab1 -> ICalculatorY: Success\n");
        }
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoLab1, (void**)&pIEcoLab1);
        if (result == 0) {
            pIEcoLab1->pVTbl->Release(pIEcoLab1);
            printf("    IEcoLab1 -> IEcoLab1: Success\n");
        }
        result = pICalculatorX->pVTbl->QueryInterface(pICalculatorX, &IID_IEcoCalculatorY, (void**)&pICalculatorY);
        if (result == 0) {
            pICalculatorY->pVTbl->Release(pICalculatorY);
            printf("    ICalculatorX -> ICalculatorY: Success\n");
        }
        result = pICalculatorX->pVTbl->QueryInterface(pICalculatorX, &IID_IEcoLab1, (void**)&pIEcoLab1);
        if (result == 0) {
            pIEcoLab1->pVTbl->Release(pIEcoLab1);
            printf("    ICalculatorX -> IEcoLab1: Success\n");
        }
        result = pICalculatorX->pVTbl->QueryInterface(pICalculatorX, &IID_IEcoCalculatorX, (void**)&pICalculatorX);
        if (result == 0) {
            pICalculatorX->pVTbl->Release(pICalculatorX);
            printf("    ICalculatorX -> ICalculatorX: Success\n");
        }
        result = pICalculatorY->pVTbl->QueryInterface(pICalculatorY, &IID_IEcoCalculatorX, (void**)&pICalculatorX);
        if (result == 0) {
            pICalculatorX->pVTbl->Release(pICalculatorX);
            printf("    ICalculatorY -> ICalculatorX: Success\n");
        }
        result = pICalculatorY->pVTbl->QueryInterface(pICalculatorY, &IID_IEcoCalculatorY, (void**)&pICalculatorY);
        if (result == 0) {
            pICalculatorY->pVTbl->Release(pICalculatorY);
            printf("    ICalculatorY -> ICalculatorY: Success\n");
        }
        result = pICalculatorY->pVTbl->QueryInterface(pICalculatorY, &IID_IEcoLab1, (void**)&pIEcoLab1);
        if (result == 0) {
            pIEcoLab1->pVTbl->Release(pIEcoLab1);
            printf("    ICalculatorY -> IEcoLab1: Success\n");
        }
    }
    system("Pause");

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

