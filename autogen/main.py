import os
import json
from jinja2 import Template as JinjaTemplate
from models import Settings
from dacite import from_dict
from utils import SYSTEM, TEMPLATE_DATA, logger, IsFileModified, UpdateFileStamp


def main():
    settings: Settings | None = None

    with open("settings.json") as f:
        settings = from_dict(data_class=Settings, data=json.loads(f.read()))

    for template in settings.templates:
        if not IsFileModified(template.filePath):
            logger.debug(
                f'Template file "{template.filePath}" has not been modified, skipping...'
            )
            continue

        templatePath = os.path.join(SYSTEM.BASE_DIR, template.filePath)

        if not os.path.exists(templatePath):
            logger.warning(
                f'Template file "{template.filePath}" does not exist, skipping...'
            )
            continue

        outputFile = template.filePath[:-3]  # remove .in extension

        if template.output is not None:
            outputFile = template.output

        with open(templatePath, "r") as f:
            templateContent = f.read()
            jinjaTemplate = JinjaTemplate(templateContent)
            renderedContent = jinjaTemplate.render(**TEMPLATE_DATA)

        with open(outputFile, "w") as f:
            f.write(renderedContent)

        logger.info(
            f'Generated file "{outputFile}" from template "{template.filePath}".'
        )

        UpdateFileStamp(template.filePath)


if __name__ == "__main__":
    main()
