import os
from models import Template
from jinja2 import Template as JinjaTemplate
from utils import SYSTEM, TEMPLATE_DATA, IsFileModified, UpdateFileStamp, logger


def GenerateTemplate(template: Template) -> str:
    """
    The tools creating the template files.

    Arguments
    ---------
    template : Template
        The template configuration.
    """
    templatePath = os.path.join(SYSTEM.BASE_DIR, template.file)

    if not os.path.exists(templatePath):
        logger.warning(f'Template file "{template.file}" does not exist, skipping...')
        return ""

    outputFile = template.file[:-3]  # remove .in extension

    if template.output is not None:
        outputFile = template.output

    fullOutputPath = os.path.join(SYSTEM.BASE_DIR, outputFile)

    if not IsFileModified(template.file) and os.path.exists(fullOutputPath):
        logger.debug(
            f'Template file "{template.file}" has not been modified, skipping...'
        )
        return ""

    with open(templatePath, "r") as f:
        templateContent = f.read()
        jinjaTemplate = JinjaTemplate(templateContent)
        renderedContent = jinjaTemplate.render(**TEMPLATE_DATA)

    with open(fullOutputPath, "w") as f:
        f.write(renderedContent)

    logger.info(f'Generated file "{outputFile}" from template "{template.file}".')

    UpdateFileStamp(template.file)

    return renderedContent
