from django.conf.urls import url
from . import views

urlpatterns = [
	url(r'^$', views.List),
	url(r'^create/$', views.Create),
	url(r'^edit/(?P<pk>\d+)/$', views.Edit, name='edit'),
	url(r'^delete/(?P<pk>\d+)/$', views.Delete, name='delete'),
]
