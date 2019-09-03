# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models

# Create your models here.
class TODO(models.Model):
	title = models.CharField(max_length=100)
	content = models.TextField(max_length=100)
	deadline = models.DateField(auto_now_add=False, auto_now=False, blank=True, null=True)