# Курсовая работа
## Программа генерации индивидуальных регистрационных знаков автомобиля
Назначение программы является генерация серии изображений индивидуальных регистрационных знаков автомобиля, предназначенной для обучения существующей нейронной сети по распознаванию индивидуального регистрационного знака автомобиля.
## Концептуальная диаграмма
![Class Diagram](/images/ClassDiagram.png)
## Диаграмма классов
![Conceptual Diagram](/images/ConceptualDiagram.png)
## Краткое руководство
Оснонвоной упор в разработке делался на редактор шаблона номера, по которому программа умеет генерировать текстуры и рендерить модели номеров. В будущем модуль рендера следует заменить на более производительный и реалистичный

### Создание шаблона
Зайдя в приложение пользователю предоставляется возможность создать шаблон текстуры автомобильного номера. Для этого используются простые объекты.
Для создания нового простого объекта необходимо нажать на кнопку «Add item» на панели инструментов. Объект появится в обозревателе редактора («Items overview»), расположенного справа.
 
Для настройки каждого простого объекта, его нужно выбрать в обозревателе редактора. Список настроек выбранного объекта отобразится слева в коне настроек. В данном окне пользователь настраивает общие параметры объекта: имя, позицию, Z-индекс, какой канал отображать в окне редактора, какой канал влияет на размер.
 
Под формой с общими параметрами располагаются настройки трех каналов объекта. Каждый канал можно включить или выключить, тем самым определяя, будет ли данный объект влиять на итоговый канал текстуры. Для каждого канала выбирается графический узел отображения: узел изображения и узел текста. По умолчанию задан узел изображения.
Каждый графический узел имеет набор параметров, некоторые их которых можно перевести в режим случайной генерации. В этом случае при генерации нового изображения, данный параметр будет изменен с помощью датчика случайных чисел.
 
Для свойства-числа в режиме случайной генерации нужно задать минимальное и максимальное значения. Для свойства-строки нужно задать маску генерации. Маска состоит из любого количества последовательностей вида “L{число}” и “D{число}”, где {число} – любое натурально число случайных символов (L - символы латинского алфавита, D - цифра), не превышающее 231-1. Любые другие символы между указанными последовательностями игнорируются.

### Генерация изображений
Для генерации изображений по созданному шаблону нужно перейти в редактор генерации изображений, нажав кнопку «Switch editors» на панели инструментов. В главном окне приложения появится редактор генерации изображений. В обозревателе редактора появятся менеджеры, управляющие процессом генерации.
 
Настройка генерации изображений происходит в менеджере рендера. Для перехода в него, нужно в обозревателе редактора выбрать «Render manager».  В окне настроек отобразятся параметры рендера: количество генерируемых изображений и путь к директории сохранения. Размер модели номера можно задать в менеджере моделей («Model manager»).
Для начала процесса генерации, пользователь должен нажать на кнопку «Render» в центре экрана. Статус генерации отображается полосой прогресса, находящаяся под кнопкой. 
