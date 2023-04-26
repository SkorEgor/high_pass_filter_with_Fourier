![C++](https://img.shields.io/badge/-C++-090909?style=for-the-badge&logo=C%2b%2b&logoColor=6296CC) ![MFC](https://img.shields.io/badge/-MFC-090909?style=for-the-badge) ![MFC](https://img.shields.io/badge/-Window_App-090909?style=for-the-badge)

<h3 align="center">Фильтрация сигнала от шума на основе преобразования Фурье</h3>
<p align="center">
Оконное приложение создает полигармонический сигнал с шумом, по заданным параметрам и позволяет произвести фильтрацию. Метод основан на переходе в частотную область (прямое преобразование Фурье), сохранение диапазона с заданной энергией и обратным переходом во временную область (обратное Фурье).
</p>

<div align="center">

![DEMO1](https://github.com/SkorEgor/picturesgifs-for-readme/blob/RobotControl/high_pass_filter_with_Fourier/1.jpg)

</div>


## Демонстрация
1)	Необходимо задать параметры исходного полигармонического сигнала. Исходный сигнал получается суммой гармоник.

 <div align="center">

![DEMO1](https://github.com/SkorEgor/picturesgifs-for-readme/blob/RobotControl/high_pass_filter_with_Fourier/2_signal_parameters.jpg)

</div>

2)	 Частота дискретизации и кол-во точек (должно быть равно 2 в степни) графика задаётся в специальном поле.

<div align="center">

![DEMO1](https://github.com/SkorEgor/picturesgifs-for-readme/blob/RobotControl/high_pass_filter_with_Fourier/3_signal_parameters.jpg)

</div>

3)	 Теперь специально зашумим сигнал, через процент шума (отношение энергий шума к сигналу). Дальше можно нажать отрисовать.

4)	 На первом графике красной линией отрисован сигнал с Гауссовым шумом, на втором спектральная плотность мощности этого сигнала, полученная прямым преобразованием Фурье (быстрым Фурье преобразованием).

<div align="center">

![DEMO1](https://github.com/SkorEgor/picturesgifs-for-readme/blob/RobotControl/high_pass_filter_with_Fourier/4_signal_and_spectrum.jpg)

</div>

5)	 Для фильтрации необходимо указать процент энергии спектра, который нужно сохранить. Дальше начиная с левой границы производим подсчет энергии, при достижении заданного значения, зануляем отсчеты (реальную и мнимую) не попавшие в диапазон.

<div align="center">

![DEMO1](https://github.com/SkorEgor/picturesgifs-for-readme/blob/RobotControl/high_pass_filter_with_Fourier/5_spectrum_filtering.jpg)

</div>

6)	От полученного спектра берем обратное Фурье преобразование, в результате имеем отфильтрованный сигнал. Дополнительно посчитана характеристика рассогласования, между результатом и исходным сигналом мез шума.

<div align="center">

![DEMO1](https://github.com/SkorEgor/picturesgifs-for-readme/blob/RobotControl/high_pass_filter_with_Fourier/6_result.jpg)

</div>

##Замечание
Теоретическая часть доступна по ссылке:  https://github.com/SkorEgor/high_pass_filter_with_Fourier/blob/master/Методы%20решения%20обратных%20задач%20в%20цифровой%20обработке%20сигн.pdf
