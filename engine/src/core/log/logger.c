#include "MEEDEngine/core/core.h"
#include "MEEDEngine/core/log/handler.h"
#include "MEEDEngine/core/log/log.h"
#include "MEEDEngine/platforms/platforms.h"
#include <stdarg.h>

/**
 * The internal log data structure. Only available inside this file.
 */
struct MdLogData
{
	struct MdLinkedList* pHandlers; ///< The linked list of log handlers.
	enum MdLogLevel		 level;		///< The current log level threshold.
};

static struct MdLogData* s_pLogData = MD_NULL; ///< The global log data instance.

void mdLogInitialize(enum MdLogLevel level)
{
	MD_ASSERT(s_pLogData == MD_NULL);
	s_pLogData = MD_MALLOC(struct MdLogData);
	mdMemorySet(s_pLogData, 0, sizeof(struct MdLogData));

	s_pLogData->pHandlers = MD_MALLOC(struct MdLinkedList);
	s_pLogData->level	  = level;
}

void mdLogAddHandler(const struct MdLogHandler* pHandler)
{
	MD_ASSERT(s_pLogData != MD_NULL);
	MD_ASSERT(pHandler != MD_NULL);
	MD_ASSERT(pHandler->recordHandle != MD_NULL);

	mdLinkedListPush(s_pLogData->pHandlers, (void*)pHandler);

	if (pHandler->init)
	{
		pHandler->init();
	}
}

void mdLogPrint(enum MdLogLevel level, const char* file, u32 line, const char* format, ...)
{
	struct MdLogRecord record;
	mdMemorySet(&record, 0, sizeof(struct MdLogRecord));

	record.file = file;
	record.line = line;

	record.level = level;
	va_list args;
	va_start(args, format);
	mdFormatString(record.message, MD_LOG_MESSAGE_MAX_LENGTH, format, args);
	va_end(args);

	struct MdLinkedListNode* pCurrent = s_pLogData->pHandlers->pHead;

	while (pCurrent != MD_NULL)
	{
		struct MdLogHandler* pHandler = (struct MdLogHandler*)pCurrent->pData;
		if (pHandler->level >= s_pLogData->level)
		{
			pHandler->recordHandle(&record);
		}
		pCurrent = pCurrent->pNext;
	}
}

void mdLogShutdown()
{
	MD_ASSERT(s_pLogData != MD_NULL);

	struct MdLinkedListNode* pCurrent = s_pLogData->pHandlers->pHead;

	while (pCurrent != MD_NULL)
	{
		struct MdLogHandler* pHandler = (struct MdLogHandler*)pCurrent->pData;

		if (pHandler->shutdown)
		{
			pHandler->shutdown();
		}

		pCurrent = pCurrent->pNext;
	}

	mdLinkedListDestroy(s_pLogData->pHandlers);
	s_pLogData->pHandlers = MD_NULL;

	MD_FREE(s_pLogData, struct MdLogData);
	s_pLogData = MD_NULL;
}