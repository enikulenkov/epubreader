/* Copyright © 2011 Євгеній Мещеряков <eugen@debian.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HORIZMOUSESWIPEGESTURERECOGNIZER_H
#define HORIZMOUSESWIPEGESTURERECOGNIZER_H

#include <QGestureRecognizer>

class HorizMouseSwipeGestureRecognizer : public QGestureRecognizer
{
public:
    explicit HorizMouseSwipeGestureRecognizer();

    virtual QGesture *create(QObject *target);
    virtual Result recognize(QGesture *state, QObject *watched, QEvent *event);
};

#endif
