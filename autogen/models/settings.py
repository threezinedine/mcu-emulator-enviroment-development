from .binding import Binding
from .template import Template
from dataclasses import dataclass


@dataclass
class Settings:
    bindings: list[Binding]
    templates: list[Template]
