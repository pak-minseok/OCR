# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from django.shortcuts import get_object_or_404, render, redirect
from django.http import HttpResponseRedirect, HttpResponse, Http404
from django.shortcuts import render
from .models import TODO
# Create your views here.
# home
def List(request):
	todolist = TODO.objects.all()
	return render(request, 'list.html', {
			'todolist': todolist,
		})

def Create(request):
	if request.method == "POST":
		title = request.POST.get("title")
		content = request.POST.get("content")
		deadline = request.POST.get("deadline")

		if deadline == "":
			deadline = None
		obj = TODO.objects.create(
			title = title,
			content = content,
			deadline = deadline,
			)
		return HttpResponseRedirect('/')
	context={}
	return render(request, 'create.html', context)

def Edit(request, pk):
	todo = get_object_or_404(TODO, pk=pk)
	
	if request.method == "POST":
		title = request.POST.get("title")
		content = request.POST.get("content")

		obj = TODO.objects.filter(pk=pk)
		obj.update(
			title = title,
			content = content,
			)
		return HttpResponseRedirect("/")
	
	context={'todo': todo}
	return render(request, 'edit.html', context)

def Delete(request, pk):
	todo = get_object_or_404(TODO, pk=pk)
	todo.delete()
	return HttpResponseRedirect("/")