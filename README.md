Красно-черное дерево
==========================

## Структура проекта

Проект структурирован в соответствии с принятыми стандартами и содержит следующие каталоги верхнего уровня:

* `/docs` — документация: задание;
* `/src` — исходные платформо-мало-или-почти-независимые коды;
* `/tests` — тесты
* `readme.md` — ридмишка с комментариями к содержимому текущего каталога в формате Markdown. Чтобы просмотреть локальную версию файла с красивым форматированием, можно открыть в Firefox с установленным каким-то там плагином.


### Описание задания
[docs/document.pdf](docs/document.pdf)

# Дедлайн:
## 15.12.19 23:59
Коммиты после этого времени будут игнорироваться.

### Дополнительно.
Раскомментируйте `//#define RBTREE_WITH_DELETION` в самом начале `rbtree.h`, 
чтобы включить тесты на удаление. (Задание обязательное.)

При тестировании вы можете визуализировать промежуточные состояния работы дерева
в формате GraphViz (`.gv`). Дампы сохраняются в папку [out/gv](out/gv). 
Чтобы увидеть картинки с деревом необходимо:
* Скачать Graphviz https://graphviz.gitlab.io/download/
* Сгенерировать `.gv` файлы с помощью тестов.
* Запустить скрипт [dot-png-all.cmd](out/dot-png-all.cmd) (для Windows).
(Проверьте путь к папке bin в скрипте.)
* Открыть сгенерированные картинки в папке [out/img](out/img).
